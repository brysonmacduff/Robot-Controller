#pragma once
#include <cstdint>
#include <array>
#include <map>
#include <cstring>
#include <sstream>
#include <iomanip>

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
    static constexpr uint8_t MAXIMUM_PAYLOAD_SIZE = 28;

    // COMMAND or STATUS
    uint8_t type {0};
    // Used for message ordering
    uint8_t sequence_number {0};
    // Specifies either the type of command or type of status
    uint8_t payload_type {0};
    uint8_t payload_size {0};
    std::array<uint8_t, MAXIMUM_PAYLOAD_SIZE> payload {0};

    std::string ToString()
    {
        std::ostringstream os;

        os << "Packet { "
        << "type=0x" << std::hex << std::setw(2) << std::setfill('0') << int(type) << ", "
        << "seq="     << std::dec << int(sequence_number) << ", "
        << "payload_type=0x" << std::hex << std::setw(2) << int(payload_type) << ", "
        << "payload_size="   << std::dec << int(payload_size) << ", "
        << "payload=[";

        for (size_t i = 0; i < payload.size(); ++i) 
        {
            os << "0x" << std::hex << std::setw(2) << std::setfill('0') << int(payload[i]);
            
            if (i + 1 < payload.size())
            {
                os << " ";
            }
        }

        os << "] }";

        return os.str();
    }
};
#pragma pack(pop)



/**
 * @brief This class includes utilities for remote control of the robot.
 * @note In general, this remote control protocol follows a master-slave design where the robot device is the slave and the remote control device is the master.
 */
class RemoteControlProtocol
{
public:
    static constexpr uint8_t PACKET_SIZE = 32;

    using SerializedPacket = std::array<uint8_t, PACKET_SIZE>;

    RemoteControlProtocol() = delete;

    static SerializedPacket Serialize(const Packet& packet);
    static Packet Deserialize(const SerializedPacket& bytes);
};

} // namespace RobotMiddleware