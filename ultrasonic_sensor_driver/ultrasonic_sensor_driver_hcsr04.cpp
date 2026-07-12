#include "ultrasonic_sensor_driver_hcsr04.h"

namespace RobotController
{
// Required for linker
std::unordered_map<uint8_t, UltrasonicSensorDriverHcsr04*> UltrasonicSensorDriverHcsr04::gpio_alert_holder_registry;

UltrasonicSensorDriverHcsr04::UltrasonicSensorDriverHcsr04(uint8_t trig_gpio, uint8_t echo_gpio)
: m_trig_gpio(trig_gpio)
, m_echo_gpio(echo_gpio)
{
}

UltrasonicSensorDriverHcsr04::~UltrasonicSensorDriverHcsr04()
{
    // Remove the reference to this instance from the static map registry
    gpio_alert_holder_registry.erase(m_echo_gpio);
}

bool UltrasonicSensorDriverHcsr04::InitializeSensor()
{
    if(gpio_alert_holder_registry.contains(m_echo_gpio))
    {
        spdlog::error("{}::{}() -> Failed to initialize because trig gpio {} is already in use.",CLASS_NAME,__func__, m_echo_gpio);
        return false;
    }
    
    gpio_alert_holder_registry[m_echo_gpio] = this;

    gpioSetMode(m_trig_gpio, PI_OUTPUT);
    gpioSetMode(m_echo_gpio, PI_INPUT);
    gpioSetAlertFunc(m_echo_gpio, GpioEdgeAlertCallback);

    return true;
}

void UltrasonicSensorDriverHcsr04::GpioEdgeAlertHandler(int gpio, int level, uint32_t tick)
{
    if(gpio != m_echo_gpio)
    {
        spdlog::error("{}::{}() -> Received incorrect gpio: {}", CLASS_NAME, __func__, gpio);
        return;
    }

    // The ultrasonic sensor scan is "in-progress" for the duration that the echo gpio pin is reporting voltage level HIGH, after a trigger pulse was sent to the sensor

    if(level == VOLTAGE_LEVEL_HIGH)
    {
        // ultrasonic echo pulse has started
        m_echo_high_tick = std::chrono::microseconds{tick};
    }
    else if(level == VOLTAGE_LEVEL_LOW)
    {
        // ultrasonic echo pulse has ended
        m_echo_low_tick = std::chrono::microseconds{tick};

        ProcessTimeOfFlightSample();
    }
    else
    {
        spdlog::error("{}::{}() -> Received unexpected voltage level: {}",CLASS_NAME,__func__,level);
    }
}

void UltrasonicSensorDriverHcsr04::GpioEdgeAlertCallback(int gpio, int level, uint32_t tick)
{
    auto sensor_it = gpio_alert_holder_registry.find(gpio);

    if(sensor_it == gpio_alert_holder_registry.end())
    {
        spdlog::error("{}::{}() -> Failed to forward gpio edge alert to ultrasonic sensor instance!", CLASS_NAME,__func__);
        return;
    }

    sensor_it->second->GpioEdgeAlertHandler(gpio,level,tick);
}

bool UltrasonicSensorDriverHcsr04::RequestRangeSample()
{
    if(m_is_sampling)
    {
        return false;
    }

    m_is_sampling = true;

    if(gpioWrite(m_trig_gpio, VOLTAGE_LEVEL_LOW) != 0)
    {
        spdlog::error("{}::{}() -> Failed to write voltage {} to gpio {}",CLASS_NAME,__func__,VOLTAGE_LEVEL_LOW,m_trig_gpio);
        return false;
    }

    gpioDelay(TRIG_GPIO_SETTLING_DELAY.count());

    if(gpioTrigger(m_trig_gpio, TRIG_GPIO_SAMPLE_PULSE_PERIOD.count(), VOLTAGE_LEVEL_HIGH) != 0)
    {
        spdlog::error("{}::{}() -> Failed to trigger voltage {} for period {}us to gpio {}",CLASS_NAME,__func__,VOLTAGE_LEVEL_HIGH,TRIG_GPIO_SAMPLE_PULSE_PERIOD.count(),m_trig_gpio);
        return false;
    }

    return true;
}

void UltrasonicSensorDriverHcsr04::ProcessTimeOfFlightSample()
{
    m_range_cache_meters = ComputeRangeMeters(m_echo_high_tick, m_echo_low_tick);
    m_is_sampling = false;
    m_range_sample_callback(m_range_cache_meters);
};

float UltrasonicSensorDriverHcsr04::ComputeRangeMeters(std::chrono::microseconds start_time, std::chrono::microseconds end_time)
{
    const std::chrono::microseconds duration = end_time - start_time;

    spdlog::debug("{}::{}() -> T1: {}us, T2: {}us, TOF: {}us",CLASS_NAME,__func__,start_time.count(),end_time.count(),duration.count());

    const float duration_seconds = static_cast<float>(duration.count()) / SECOND_TO_MICROSECOND;

    // The distance in meters must be divided by two because the time-of-flight requires the sound wave to travel twice the true distance.
    const float range_meters = duration_seconds * SOUND_SPEED_METER_PER_SECOND / 2.0f;

    return range_meters;
}

} // namespace RobotController