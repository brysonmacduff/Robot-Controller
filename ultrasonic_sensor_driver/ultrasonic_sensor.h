#pragma once
#include "i_ultrasonic_sensor_driver.h"
#include "frame_types.h"
#include <memory>
#include <functional>

namespace RobotController
{

/**
 * @brief This class exists to model an ultrasonic range sensor in addition to its real-world orientation.
 */
class UltrasonicSensor
{
public:
    using DirectionCallback = std::function<EnuUnitVector()>;

    struct Attributes
    {
        double signal_frequency_hertz;
    };

    Attributes HCS04_ATTRIBUTES = { .signal_frequency_hertz = 40000 };

    UltrasonicSensor(std::unique_ptr<IUltrasonicSensorDriver> ultrasonic_sensor_driver_ptr, Attributes attributes)
    : m_ultrasonic_sensor_driver_ptr(std::move(ultrasonic_sensor_driver_ptr))
    , m_attributes(attributes)
    {
    }

    void SetDirectionCallback(DirectionCallback callback) { m_direction_callback = std::move(callback); }
    void RefreshDirection() { m_direction = m_direction_callback(); }
    const EnuUnitVector& GetDirection() { return m_direction; }
    IUltrasonicSensorDriver* GetUltrasonicSensorDriver() { return m_ultrasonic_sensor_driver_ptr.get(); }
    const Attributes& GetAttributes() const { return m_attributes; }

private:
    DirectionCallback m_direction_callback = [this](){ return m_direction; };
    std::unique_ptr<IUltrasonicSensorDriver> m_ultrasonic_sensor_driver_ptr = nullptr;
    EnuUnitVector m_direction = { .azimuth_degrees = -999, .elevation_degrees = -999 };
    Attributes m_attributes = { .signal_frequency_hertz = -999 };
};
} // namespace RobotController