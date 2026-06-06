#pragma once
#include <cstdint>

namespace RobotController
{

class Configuration
{
public:

    struct TrackChassis
    {
        uint8_t left_track_motor_gpio {0};
        uint8_t right_track_motor_gpio {0};
    };

    Configuration() = delete;

    inline static const TrackChassis DEFAULT_TRACK_CHASSIS { 16, 12 };
};
} // namespace RobotController