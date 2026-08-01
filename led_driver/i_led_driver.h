#pragma once

namespace RobotController
{
class ILedDriver
{
public:
    virtual ~ILedDriver() = default;

    /**
     * @brief Turn on the LED and set it to full luminosity.
     */
    virtual bool Enable() = 0;
    /**
     * @brief Turn off the LED
     */
    virtual bool Disable() = 0;
};
} // namespace RobotController