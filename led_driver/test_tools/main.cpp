#include "led_driver.h"
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>

using namespace RobotController;

constexpr uint8_t LED_GPIO = 8;
constexpr std::chrono::milliseconds BLINK_INTERVAL {100};

int main()
{
    if(gpioInitialise() < 0)
    {
        spdlog::error("Failed to intialize GPIO hardware");
        return 1;
    }

    LedDriverFactory factory;
    std::optional<LedDriver> led_driver_opt = factory.Build(LED_GPIO);

    if(not led_driver_opt.has_value())
    {
        spdlog::error("Failed to build the LED Driver");
        return 1;
    }

    LedDriver& led_driver = led_driver_opt.value();

    while(true)
    {
        led_driver.Enable();
        std::this_thread::sleep_for(BLINK_INTERVAL);
        led_driver.Disable();
        std::this_thread::sleep_for(BLINK_INTERVAL);
    }

    return 0;
}