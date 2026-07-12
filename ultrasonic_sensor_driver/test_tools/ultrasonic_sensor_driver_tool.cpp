#include "ultrasonic_sensor_driver_hcsr04.h"
#include <thread>

using namespace RobotController;

static constexpr std::chrono::milliseconds SAMPLE_DELAY {100};
static constexpr uint8_t TRIG_GPIO = 26;
static constexpr uint8_t ECHO_GPIO = 13;

int main()
{
    if(gpioInitialise() < 0)
    {
        spdlog::error("Failed to initialize GPIO hardware!");
        return 1;
    }

    UltrasonicSensorDriverHcsr04 sensor(TRIG_GPIO,ECHO_GPIO);

    sensor.SetRangeSampleCallback([](float range_meters)
    {
        spdlog::info("Range (meters): {}", range_meters);
    });

    sensor.InitializeSensor();

    sensor.RequestRangeSample();

    std::this_thread::sleep_for(SAMPLE_DELAY);

    gpioTerminate();

    return 0;
};