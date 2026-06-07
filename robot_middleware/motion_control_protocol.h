#pragma once
#include "remote_control_protocol.h"

#include <optional>

namespace RobotMiddleware
{

/**
 * @brief This class includes utilities for remote control of a robot propulsion system.
 */
class MotionControlProtocol
{
public:

    MotionControlProtocol() = delete;

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

    enum class CommandType
    {
        ENACT_MOTION
    };

    inline static const std::map<CommandType, uint8_t> COMMAND_TYPE_CODES = {
        {CommandType::ENACT_MOTION, 0x00}  
    };

    enum class StatusType
    {
        MOTION_STATE
    };

    inline static const std::map<StatusType, uint8_t> STATUS_TYPE_CODES = {
        {StatusType::MOTION_STATE, 0x00}
    };

    /* High Level Message Structures ---------------------------------- */

    struct MotionCommand
    {
        MotionControlProtocol::MotionState motion_state {};
        uint8_t sequence_number {0};
    };

    struct MotionStatus
    {
        MotionState motion_state {};
        uint8_t sequence_number {0};
    };

    /* ----------------------------------------------------------------- */

    static Packet BuildMotionCommandPacket(const MotionState& motion_command, uint8_t sequence_number);
    static Packet BuildMotionStatusPacket(const MotionState& motion_status, uint8_t sequence_number);
    
    /**
     * @brief Indicates whether a packet contains valid information.
     * @note Packet validity is determined based on the tracked chassis protocol.
     */
    static bool IsPacketValid(const Packet& packet);

    /**
     * @brief Determines whether the received packet is a valid motion command message.
     */
    static bool IsMotionCommandPacket(const Packet& packet);

     /**
     * @brief Determines whether the received packet is a valid motion status message.
     */
    static bool IsMotionStatusPacket(const Packet& packet);

    /**
     * @brief Extracts the essential motion command data from the packet. Strips out lower-level protocol data.
     * @returns Empty optional type if extraction fails due to an invalid packet or incompatible packet type. "Incompatible" would refer to a non-motion command packet.
     */
    static std::optional<MotionCommand> ExtractMotionCommand(const Packet& packet);

    /**
     * @brief Extracts the essential motion status data from the packet, dropping the lower-level protocol data in the packet.
     * @returns Empty optional type if extraction fails due to an invalid packet for incompatible packet type. "Incompatible" would refer to a non-motion status packet.
     */
    static std::optional<MotionStatus> ExtractMotionStatus(const Packet& packet);

private:
    static bool IsPacketTypeValid(const Packet& packet);
    static bool IsPayloadTypeValid(const Packet& packet);
    static bool IsPayloadSizeValid(const Packet& packet);
    static bool IsCommandPayloadTypeValid(uint8_t type);
    static bool IsStatusPayloadTypeValid(uint8_t type);
    static bool IsMotionStateCodeValid(uint8_t motion_state_code);

};
} // namespace RobotMiddleware