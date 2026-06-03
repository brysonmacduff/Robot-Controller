#include "simple_motor_driver.h"

#include <thread>
#include <chrono>
#include <gtest/gtest.h>

namespace
{
using namespace RobotController;

TEST(SimpleMotorDriverTest, ActivateAndDeactivateMotor)
{
    gpioInitialise();

    const uint8_t gpio = 16;

    SimpleMotorDriver driver = SimpleMotorDriver(gpio);

    for(uint8_t count = 0; count < 10; ++count)
    {
        EXPECT_TRUE(driver.ActivateMotor());
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
        EXPECT_TRUE(driver.DeactivateMotor());
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }

    gpioTerminate();
}

}