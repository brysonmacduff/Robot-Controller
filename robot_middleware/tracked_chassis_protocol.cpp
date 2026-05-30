#include "tracked_chassis_protocol.h"

namespace RobotMiddleware
{

Packet TrackedChassisProtocol::BuildMotionCommandPacket(const MotionState& motion_command, uint8_t sequence_number)
{
    Packet packet {};

    packet.type = Packet::COMMAND;
    packet.payload_type = CommandType::ENACT_MOTION;
    packet.payload_size = 1;
    packet.payload[0] = MOTION_STATE_CODES.at(motion_command);
    packet.sequence_number = sequence_number;

    return packet;
}

Packet TrackedChassisProtocol::BuildMotionStatusPacket(const MotionState &motion_status, uint8_t sequence_number)
{
    Packet packet {};

    packet.type = Packet::STATUS;
    packet.payload_type = StatusType::MOTION_STATE;
    packet.payload_size = 1;
    packet.payload[0] = MOTION_STATE_CODES.at(motion_status);
    packet.sequence_number = sequence_number;

    return packet;
}

} // namespace RobotMiddleware