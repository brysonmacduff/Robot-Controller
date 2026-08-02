#include "led_driver.h"
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>

using namespace RobotController;

constexpr uint8_t LED_GPIO = 26;
constexpr std::chrono::milliseconds BLINK_INTERVAL {100};

int main()
{
    if(gpioInitialise() < 0)
    {
        spdlog::error("Failed to intialize GPIO hardware");
        return 1;
    }

    LedDriverFactory factory;
    std::unique_ptr<LedDriver> led_driver_ptr = factory.Build(LED_GPIO);

    if(led_driver_ptr == nullptr)
    {
        spdlog::error("Failed to build the LED Driver");
        return 1;
    }

    while(true)
    {
        led_driver_ptr->Enable();
        std::this_thread::sleep_for(BLINK_INTERVAL);
        led_driver_ptr->Disable();
        std::this_thread::sleep_for(BLINK_INTERVAL);
    }

    return 0;
}