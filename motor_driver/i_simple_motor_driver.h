#pragma once

namespace RobotController
{
class ISimpleMotorDriver
{
public:
    virtual ~ISimpleMotorDriver() = default;

    virtual bool ActivateMotor() = 0;
    virtual bool DeactivateMotor() = 0;
    virtual bool IsMotorActive() const = 0;
};
} // namespace RobotController