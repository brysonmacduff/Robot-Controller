#include "simple_motor_driver.h"

namespace RobotController 
{
SimpleMotorDriver::~SimpleMotorDriver()
{
    DeactivateMotor();
}

SimpleMotorDriver::SimpleMotorDriver(uint8_t gpio) : m_gpio(gpio)
{
}

bool SimpleMotorDriver::ActivateMotor()
{
    // Ignore activation if the motor is already active.
    if(IsMotorActive())
    {
        return true;
    }

    const bool result = gpioWrite(m_gpio, PI_HIGH) == 0;

    if(not result)
    {
        return false;
    }

    m_is_motor_active = true;

    return true;
}

bool SimpleMotorDriver::DeactivateMotor()
{
    // Ignore deactivation if the motor is already inactive.
    if(not IsMotorActive())
    {
        return true;
    }

    const bool result = gpioWrite(m_gpio, PI_LOW) == 0;

    if(not result)
    {
        return false;
    }

    m_is_motor_active = false;

    return true;
}

bool SimpleMotorDriver::IsMotorActive() const
{
    return m_is_motor_active;
}

} // namespace RobotController