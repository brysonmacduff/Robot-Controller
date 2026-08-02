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

std::unique_ptr<LedDriver> LedDriverFactory::Build(uint8_t led_gpio)
{
    m_led_driver_ptr = std::unique_ptr<LedDriver>(new LedDriver(led_gpio));

    if(not m_led_driver_ptr->Initialize())
    {
        return nullptr;
    }

    return std::move(m_led_driver_ptr);
}

void LedDriverFactory::Reset()
{
    m_led_driver_ptr.reset();
}

} // namespace RobotController