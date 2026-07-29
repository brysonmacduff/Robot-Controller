#pragma once
#include <functional>

namespace RobotController
{
class IUltrasonicSensorDriver
{
public:
    virtual ~IUltrasonicSensorDriver() = default;

    /**
     * @brief Requests that the ultrasonic sensor starts a range sample.
    */
    virtual bool RequestRangeSample() = 0;

    /**
     * @brief Allows a listener to set a callback so they are given the range data from a sample once complete.
     * @note The range callback must report the range in meters.
    */
    virtual void SetRangeSampleCallback(std::function<void(float range_meters)> callback) = 0;

    /**
     * @brief Request that the sensor hardware initializes.
     * @returns Whether the hardware initialization succeeded.
     */
    virtual bool InitializeSensor() = 0;
};
} // namespace RobotController