#pragma once
#include "motion_control_protocol.h"

#include <functional>

namespace RobotMiddleware
{

using MotionCommand = MotionControlProtocol::MotionCommand;

class CommandSender
{
public:
    using SenderCallback = std::function<void(const Packet& packet)>;

    CommandSender() = default;

    /**
     * @brief Set a callback function that this class calls when it wants to send a packet.
     */
    void SetSenderCallback(SenderCallback callback) { m_sender_callback = std::move(callback); }

     /**
     * @brief Send a motion command message
     */
    void Send(const MotionCommand& motion_command) 
    {
        const Packet motion_command_packet = MotionControlProtocol::BuildMotionCommandPacket(motion_command.motion_state, m_sequence_number);

        m_sender_callback(motion_command_packet);

        ++m_sequence_number;
    }
    
private:

    uint8_t m_sequence_number {0};
    SenderCallback m_sender_callback = [](const Packet& packet){(void)packet;};

};
} // namespace RobotMiddleware