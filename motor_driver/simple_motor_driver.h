#pragma once
#include "i_simple_motor_driver.h"
#include <cstdint>
#include <pigpio.h>

namespace RobotController
{
/**
 * @brief SimpleMotorDriver is a basic motor control class that provides simple on-off switch control of a motor.
 * @note It is assumed that this driver is controlling gpio pin that is attacked to a relay that managed the power supply to a motor.
 * @warning 
 * - Requires root privilege to use gpio hardware. 
 * - This class is not responsible for gpio hardware management. It will not work properly if gpio hardware is not initialized.
 */
class SimpleMotorDriver : public ISimpleMotorDriver
{
public:
    ~SimpleMotorDriver();
    SimpleMotorDriver(uint8_t gpio);

    /**
     * @brief Enables a motor.
     */
    bool ActivateMotor() override;
    
    /**
     * @brief Disables a motor.
     * @returns Whether the motor gpio signal was set to active.
     */
    bool DeactivateMotor() override;
    
    /**
     * @brief Shows whether the motor is currently supplied with power.
     * @returns Whether the motor gpio signal was set to inactive.
     */
    bool IsMotorActive() const override;

private:

    uint8_t m_gpio { 0 };
    bool m_is_motor_active { false };

};
} // namespace RobotController