#include "ultrasonic_sensor_driver_hcsr04.h"
#include <gtest/gtest.h>

namespace
{
using namespace RobotController;

class UltrasonicSensorDriverHcsr04Test : public ::testing::Test
{
public:
    static constexpr uint8_t TRIG_GPIO = 26;
    static constexpr uint8_t ECHO_GPIO = 13;
    static constexpr std::chrono::milliseconds RANGE_SAMPLE_DELAY {100};

    void SetUp() override
    {
        ASSERT_GE(gpioInitialise(), 0);
    }

    void TearDown() override
    {
        gpioTerminate();
    }
};

/**
 * @warning This is a hardware-dependent unit test. Do not add this to ctest.
*/
TEST_F(UltrasonicSensorDriverHcsr04Test, RequestRangeSample)
{
    UltrasonicSensorDriverHcsr04 sensor(TRIG_GPIO,ECHO_GPIO);

    bool callback_activated = false;
    float range_sample = -1;

    sensor.SetRangeSampleCallback([&](float range_meters)
    {
        spdlog::info("Range (meters): {}", range_meters);
        callback_activated = true;
        range_sample = range_meters;
    });

    ASSERT_TRUE(sensor.InitializeSensor());

    EXPECT_TRUE(sensor.RequestRangeSample());

    int sleep_counter = 0;
    const int sleep_limit = 3;
    while(not callback_activated)
    {
        std::this_thread::sleep_for(RANGE_SAMPLE_DELAY);
        ++sleep_counter;

        if(sleep_counter == sleep_limit)
        {
            break;
        }
    }

    EXPECT_TRUE(callback_activated);
    EXPECT_FLOAT_EQ(range_sample, sensor.GetCachedRangeMeters());
}

} // namespace