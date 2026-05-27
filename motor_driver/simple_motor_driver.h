#pragma once
#include <cstdint>

namespace RobotController
{
/**
 * @brief SimpleMotorDriver is a basic motor control class that provides simple on-off switch control of a motor.
 */
class SimpleMotorDriver
{
public:
    ~SimpleMotorDriver() = default;
    SimpleMotorDriver(uint8_t gpio);

    void ActivateMotor();
    void DeactivateMotor();
    
    bool IsMotorActive() const;

private:

    uint8_t m_gpio { 0 };
    bool m_is_motor_active { false };
};
} // namespace RobotController