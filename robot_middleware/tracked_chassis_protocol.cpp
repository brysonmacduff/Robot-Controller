#include "tracked_chassis_protocol.h"

namespace RobotMiddleware
{

Packet TrackedChassisProtocol::BuildMotionCommandPacket(const MotionState& motion_command, uint8_t sequence_number)
{
    Packet packet {};

    packet.type = Packet::COMMAND;
    packet.payload_type = COMMAND_TYPE_CODES.at(CommandType::ENACT_MOTION);
    packet.payload_size = 1;
    packet.payload[0] = MOTION_STATE_CODES.at(motion_command);
    packet.sequence_number = sequence_number;

    return packet;
}

Packet TrackedChassisProtocol::BuildMotionStatusPacket(const MotionState &motion_status, uint8_t sequence_number)
{
    Packet packet {};

    packet.type = Packet::STATUS;
    packet.payload_type = STATUS_TYPE_CODES.at(StatusType::MOTION_STATE);
    packet.payload_size = 1;
    packet.payload[0] = MOTION_STATE_CODES.at(motion_status);
    packet.sequence_number = sequence_number;

    return packet;
}

bool TrackedChassisProtocol::IsPacketValid(const Packet &packet)
{
    return IsPacketTypeValid(packet) && IsPayloadTypeValid(packet) && IsPayloadSizeValid(packet);
}

bool TrackedChassisProtocol::IsPacketTypeValid(const Packet &packet)
{
    return packet.type == Packet::COMMAND || packet.type == Packet::STATUS;
}

bool TrackedChassisProtocol::IsPayloadTypeValid(const Packet &packet)
{
    bool is_valid = false;

    if(packet.type == Packet::COMMAND)
    {
        is_valid = IsCommandPayloadTypeValid(packet.payload_type);
    }
    else if(packet.type == Packet::STATUS)
    {
        is_valid = IsStatusPayloadTypeValid(packet.payload_type);
    }

    return is_valid;
}

bool TrackedChassisProtocol::IsPayloadSizeValid(const Packet &packet)
{
    return packet.payload_size <= Packet::MAXIMUM_PAYLOAD_SIZE;
}

bool TrackedChassisProtocol::IsCommandPayloadTypeValid(uint8_t type)
{
    bool is_valid = false;

    for(auto pair : COMMAND_TYPE_CODES)
    {
        if(type == pair.second)
        {
            is_valid = true;
            break;
        }
    }

    return is_valid;
}

bool TrackedChassisProtocol::IsStatusPayloadTypeValid(uint8_t type)
{
    bool is_valid = false;

    for(auto pair : STATUS_TYPE_CODES)
    {
        if(type == pair.second)
        {
            is_valid = true;
            break;
        }
    }

    return is_valid;
}

} // namespace RobotMiddleware