#include "motion_control_protocol.h"

namespace RobotMiddleware
{

using MotionCommand = MotionControlProtocol::MotionCommand;

Packet MotionControlProtocol::BuildMotionCommandPacket(const MotionState& motion_command, uint8_t sequence_number)
{
    Packet packet {};

    packet.type = Packet::COMMAND;
    packet.payload_type = COMMAND_TYPE_CODES.at(CommandType::ENACT_MOTION);
    packet.payload_size = 1;
    packet.payload[0] = MOTION_STATE_CODES.at(motion_command);
    packet.sequence_number = sequence_number;

    return packet;
}

Packet MotionControlProtocol::BuildMotionStatusPacket(const MotionState &motion_status, uint8_t sequence_number)
{
    Packet packet {};

    packet.type = Packet::STATUS;
    packet.payload_type = STATUS_TYPE_CODES.at(StatusType::MOTION_STATE);
    packet.payload_size = 1;
    packet.payload[0] = MOTION_STATE_CODES.at(motion_status);
    packet.sequence_number = sequence_number;

    return packet;
}

bool MotionControlProtocol::IsPacketValid(const Packet &packet)
{
    return IsPacketTypeValid(packet) && IsPayloadTypeValid(packet) && IsPayloadSizeValid(packet);
}

bool MotionControlProtocol::IsMotionCommandPacket(const Packet& packet)
{
    if(not IsPacketValid(packet))
    {
        return false;
    }

    bool result = true;

    result &= packet.type == Packet::COMMAND;
    result &= packet.payload_type == COMMAND_TYPE_CODES.at(CommandType::ENACT_MOTION);
    result &= packet.payload_size == 1;
    
    const uint8_t motion_state_code = packet.payload[0];

    result &= IsMotionStateCodeValid(motion_state_code);

    return result;
}

bool MotionControlProtocol::IsMotionStatusPacket(const Packet& packet)
{
    if(not IsPacketValid(packet))
    {
        return false;
    }

    bool result = true;

    result &= packet.type == Packet::STATUS;
    result &= packet.payload_type == STATUS_TYPE_CODES.at(StatusType::MOTION_STATE);
    result &= packet.payload_size == 1;

    const uint8_t motion_state_code = packet.payload[0];

    result &= IsMotionStateCodeValid(motion_state_code);

    return result;
}

std::optional<MotionCommand> MotionControlProtocol::ExtractMotionCommand(const Packet &packet)
{
    if(not IsMotionCommandPacket(packet))
    {
        return std::nullopt;
    }

    MotionCommand motion_command {};

    const uint8_t motion_state_code = packet.payload[0];

    for(auto pair : MOTION_STATE_CODES)
    {
        if(pair.second == motion_state_code)
        {
            motion_command.motion_state = pair.first;
            break;
        }
    }

    return motion_command;
}

std::optional<MotionControlProtocol::MotionStatus> MotionControlProtocol::ExtractMotionStatus(const Packet& packet)
{
    if(not IsMotionStatusPacket(packet))
    {
        return std::nullopt;
    }

    MotionStatus motion_status {};

    const uint8_t motion_state_code = packet.payload[0];

    for(auto pair : MOTION_STATE_CODES)
    {
        if(pair.second == motion_state_code)
        {
            motion_status.motion_state = pair.first;
            break;
        }
    }

    return motion_status;
}

bool MotionControlProtocol::IsPacketTypeValid(const Packet &packet)
{
    return packet.type == Packet::COMMAND || packet.type == Packet::STATUS;
}

bool MotionControlProtocol::IsPayloadTypeValid(const Packet &packet)
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

bool MotionControlProtocol::IsPayloadSizeValid(const Packet &packet)
{
    return packet.payload_size <= Packet::MAXIMUM_PAYLOAD_SIZE;
}

bool MotionControlProtocol::IsCommandPayloadTypeValid(uint8_t type)
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

bool MotionControlProtocol::IsStatusPayloadTypeValid(uint8_t type)
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

bool MotionControlProtocol::IsMotionStateCodeValid(uint8_t motion_state_code)
{
    bool is_valid = false;

    for(auto pair : MOTION_STATE_CODES)
    {
        if(motion_state_code == pair.second)
        {
            is_valid = true;
            break;
        }
    }

    return is_valid;
}

} // namespace RobotMiddleware