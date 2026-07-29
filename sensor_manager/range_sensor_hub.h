#pragma once
#include "i_range_sensor_hub.h"
#include "i_ultrasonic_sensor_driver.h"
#include "ultrasonic_sensor.h"
#include "frame_types.h"

#include <optional>
#include <memory>
#include <vector>

namespace RobotController
{
// Forward declare the factory class
class RangeSensorHubFactory;

class RangeSensorHub : public IRangeSensorHub
{
public:
    RangeSensorHub() = delete;

    void SetRangeSampleCallback(RangeSampleCallback callback) override;
    void Sample() override;

private:

    friend RangeSensorHubFactory;

    RangeSampleCallback m_range_sample_callback = [](const EnuVector& range_sample_vector){ (void)range_sample_vector; };
    
    // This class is meant to support multiple collections of range sensor types.
    // Differentiating by the sensor type is important for correctly interpreting the sensor data.
    std::vector<UltrasonicSensor> m_ultrasonic_sensors;
};

/**
 * @brief This class functions as a factory for the range sensor hub class.
 */
class RangeSensorHubFactory
{
public:

    void WithUltrasonicSensor(UltrasonicSensor&& ultrasonic_sensor)
    {
        m_range_sensor_hub_ptr->m_ultrasonic_sensors.push_back(std::move(ultrasonic_sensor));
    }

    /**
     * @brief Resets the range sensor hub under construction to a null state.
     */
    void Reset() { m_range_sensor_hub_ptr.reset(); }

    /**
     * @brief Attempts to create a range sensor hub and initialize the range sensors (requires hardware configuration to be valid).
     * @warning This function will attempt to initialize all of the sensors that have been added. This can fail.
     * @returns A a constructed range sensor hub if successful, nullptr otherwise.
     */
    std::unique_ptr<RangeSensorHub> Build();

private:

    std::unique_ptr<RangeSensorHub> m_range_sensor_hub_ptr = nullptr;
};

} // namespace RobotController