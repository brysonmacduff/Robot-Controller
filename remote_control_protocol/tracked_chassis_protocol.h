#pragma once
#include "remote_control_protocol.h"

namespace RobotMiddleware
{
/**
 * @brief This class includes utilities for remote control of a robot with a tracked chassis for propulsion.
 */
class TrackedChassisProtocol
{
public:
    TrackedChassisProtocol() = delete;

    enum class MotionState
    {
        IDLE,
        TRANSLATE_FORWARD,
        ROTATE_CLOCKWISE,
        ROTATE_ANTICLOCKWISE
    };
    
    inline static const std::map<MotionState, uint8_t> MOTION_STATE_CODES = { 
        {MotionState::IDLE, 0x00},
        {MotionState::TRANSLATE_FORWARD, 0x01},
        {MotionState::ROTATE_CLOCKWISE, 0x02},
        {MotionState::ROTATE_ANTICLOCKWISE, 0x03}
    };

    class CommandType
    {
    public:
        CommandType() = delete;
        static constexpr uint8_t ENACT_MOTION = 0x00;
    };

    class StatusType
    {
    public:
        StatusType() = delete;
        static constexpr uint8_t MOTION_STATE = 0x00;
    };

    static Packet BuildMotionCommandPacket(const MotionState& motion_command, uint8_t sequence_number);
    static Packet BuildMotionStatusPacket(const MotionState& motion_status,uint8_t sequence_number);
};
} // namespace RobotMiddleware