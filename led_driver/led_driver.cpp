#include "led_driver.h"

namespace RobotController
{
LedDriver::LedDriver(uint8_t led_gpio) : m_led_gpio(led_gpio)
{
}

bool LedDriver::Initialize()
{
    return 0 == gpioSetMode(m_led_gpio, PI_OUTPUT);
}

bool LedDriver::Enable()
{
    return 0 == gpioWrite(m_led_gpio, PI_HIGH);
}

bool LedDriver::Disable()
{
    return 0 == gpioWrite(m_led_gpio, PI_LOW);
}

std::optional<LedDriver> LedDriverFactory::Build(uint8_t led_gpio)
{
    LedDriver led_driver(led_gpio);

    if(not led_driver.Initialize())
    {
        return std::nullopt;
    }

    return led_driver;
}

} // namespace RobotController