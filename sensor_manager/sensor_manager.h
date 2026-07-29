#pragma once
#include "i_range_sensor_hub.h"

namespace RobotController
{
class SensorManager
{
public:
    SensorManager(IRangeSensorHub& range_sensor_hub)
    : m_range_sensor_hub(range_sensor_hub)
    {}

private:

    IRangeSensorHub& m_range_sensor_hub;
};
} // namespace RobotController