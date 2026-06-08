#pragma once
#include "motion_control_protocol.h"

#include <functional>

namespace RobotMiddleware
{

using MotionStatus = MotionControlProtocol::MotionStatus;

class StatusSender
{
public:
    using SenderCallback = std::function<void(const Packet& packet)>;

    StatusSender() = default;

    /**
     * @brief Set a callback function that this class calls when it wants to send a packet.
     */
    void SetSenderCallback(SenderCallback callback) { m_sender_callback = std::move(callback); }

    /**
     * @brief Send a motion status message
     */
    void Send(const MotionStatus& motion_status) 
    {
        const Packet motion_status_packet = MotionControlProtocol::BuildMotionStatusPacket(motion_status.motion_state, m_sequence_number);

        m_sender_callback(motion_status_packet);

        ++m_sequence_number;
    }

private:
    uint8_t m_sequence_number {0};
    SenderCallback m_sender_callback = [](const Packet& packet) {(void)packet;};
};

} // namespace RobotMiddleware