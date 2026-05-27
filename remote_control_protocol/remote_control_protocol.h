#pragma once
#include <cstdint>
#include <array>
#include <map>

namespace RobotMiddleware
{

/**
 * @brief This structure represents the structure of a 32 byte packet that is used for remote control of the robot device.
 */
#pragma pack(push, 1)
struct Packet
{
    static constexpr uint8_t COMMAND = 0xAB;
    static constexpr uint8_t STATUS = 0xBA;

    // COMMAND or STATUS
    uint8_t type {0};
    // Used for message ordering
    uint8_t sequence_number {0};
    // Specifies either the type of command or type of status
    uint8_t payload_type {0};
    uint8_t payload_length {0};
    std::array<uint8_t, 28> payload {0};
};
#pragma pack(pop)

/**
 * @brief This class includes utilities for remote control of the robot.
 * @note In general, this remote control protocol follows a master-slave design where the robot device is the slave and the remote control device is the master.
 */
class RemoteControlProtocol
{
public:
    RemoteControlProtocol() = delete;

    using PacketBytes = std::array<uint8_t, PACKET_SIZE>;

    static constexpr uint8_t PACKET_SIZE = 32;

    static std::array<uint8_t, PACKET_SIZE> Serialize(const Packet& packet);
    static Packet Deserialize(const PacketBytes& bytes);
};

} // namespace RobotMiddleware