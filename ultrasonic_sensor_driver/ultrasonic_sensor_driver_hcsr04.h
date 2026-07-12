#pragma once
#include "i_ultrasonic_sensor_driver.h"
#include "si_units.h"

#include <string_view>
#include <chrono>
#include <unordered_map>
#include <spdlog/spdlog.h>
#include <pigpio.h>

namespace RobotController
{
/**
 * @brief This class functions a driver for the HC-SR04 ultrasonic sensor.
 * @warning This class assumes it is given the correct gpio configuration for control of an HC-SR04 ultrasonic sensor.
 * - This class does not take responsibilty for overall GPIO hardware initialization. However, it will configure its assigned gpio pins as needed.
 * - This class contains static singleton-like behaviour. Once assigned, no other classes can use the trig or echo gpios. 
 * - This class is unaware of other users of gpio hardware and does not take responsibility for resolving gpio conflicts.
*/
class UltrasonicSensorDriverHcsr04 : public IUltrasonicSensorDriver
{
public:
    UltrasonicSensorDriverHcsr04(uint8_t trig_gpio, uint8_t echo_gpio);
    ~UltrasonicSensorDriverHcsr04();

    /**
     * @brief Orders the ultrasonic sensor to start a scan that will determine the distance in front of it.
     * @note It takes real time for the sensor to determine the distance, but this a very fast operation (microseconds).
     * @returns Whether the sample request was granted. The request may be denied if a previous sample request was in progress.
    */
    bool RequestRangeSample() override;

    /**
     * @brief Allows a listener to set a callback so they are given the range data from a sample once complete.
     * @param callback - When activated, the callback provides the range in meters.
    */
    void SetRangeSampleCallback(std::function<void(float range_meters)> callback) override
    {
        m_range_sample_callback = std::move(callback);
    };

    /**
     * @brief Initializes communication with the sensor device and prepares to service range sample requests.
     */
    bool InitializeSensor();

    /**
     * @brief The last known distance that the ultrasonic sensor reported.
     */
     float GetCachedRangeMeters() { return m_range_cache_meters; }

private:
    static constexpr std::string_view CLASS_NAME = "UltrasonicSensorDriverHcsr04";
    static constexpr int VOLTAGE_LEVEL_HIGH = 1;
    static constexpr int VOLTAGE_LEVEL_LOW = 0;
    // The sensor hardware requires the trig gpio to settle on voltage level low for 2 microseconds before the high voltage level pulse can begin
    static constexpr std::chrono::microseconds TRIG_GPIO_SETTLING_DELAY {2};
    // The sensor hardware requires a 10 microsecond voltage level high pulse to begin the time-of-flight ultrasonic sensor scan
    static constexpr std::chrono::microseconds TRIG_GPIO_SAMPLE_PULSE_PERIOD {10};
    // This is the speed of sound assuming a temperature of 20 degrees celcius
    static constexpr float SOUND_SPEED_METER_PER_SECOND = 343;

    // Holds a mapping of echo gpio pins to their respective ultrasonic sensor class instances
    static std::unordered_map<uint8_t, UltrasonicSensorDriverHcsr04*> gpio_alert_holder_registry;

    // Holds the trigger gpio that is used for sending scan-start pulses to the ultrasonic sensor
    uint8_t m_trig_gpio { 0 };
    // Holds the echo gpio that is used to read the time-of-flight of the ultrasonic sound pulse
    uint8_t m_echo_gpio { 0 };
    // Stores the last sampled distance in centimeters
    float m_range_cache_meters { -1 };
    // Indicates whether the sensor is currently scanning
    bool m_is_sampling { false };

    std::function<void(float)> m_range_sample_callback = [&](float range){ spdlog::info("{}::RangeSampleCallback() -> Range: {}", CLASS_NAME, range); };
    
    std::chrono::microseconds m_echo_high_tick { 0 };
    std::chrono::microseconds m_echo_low_tick { 0 };

    // This function is a callback that should be activated when the selected gpio switches from its voltage level
    void GpioEdgeAlertHandler(int gpio, int level, uint32_t tick);
    void ProcessTimeOfFlightSample();
    static float ComputeRangeMeters(std::chrono::microseconds start_time, std::chrono::microseconds end_time);
    /**
     * @brief This function is registered as a callback for a gpio alert. It will forward the alert to the a specific instance of this class.
     * @note This is needed because only static functions can be registered as gpio alert callbacks, and this class is not a static singleton.
    */ 
    static void GpioEdgeAlertCallback(int gpio, int level, uint32_t tick);

};
} // namespace RobotController