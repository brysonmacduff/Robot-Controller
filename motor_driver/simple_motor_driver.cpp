#include "simple_motor_driver.h"

namespace RobotController 
{
SimpleMotorDriver::SimpleMotorDriver(uint8_t gpio) : m_gpio(gpio)
{
}

void SimpleMotorDriver::ActivateMotor()
{
    // Ignore activation if the motor is already active.
    if(IsMotorActive())
    {
        return;
    }
}

void SimpleMotorDriver::DeactivateMotor()
{
    // Ignore deactivation if the motor is already inactive.
    if(not IsMotorActive())
    {
        return;
    }
}

bool SimpleMotorDriver::IsMotorActive() const
{
    return m_is_motor_active;
}

} // namespace RobotController