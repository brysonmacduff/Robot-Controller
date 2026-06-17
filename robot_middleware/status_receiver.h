#pragma once
#include "motion_control_protocol.h"

#include <spdlog/spdlog.h>

#include <string_view>
#include <functional>


namespace RobotMiddleware
{

using MotionStatus = MotionControlProtocol::MotionStatus;

class StatusReceiver
{
public:
    using MotionStatusReceiverCallback = std::function<void(const MotionStatus& motion_status)>;

    StatusReceiver() = default;

    /**
     * @brief Receives a packet and conditionally extracts the status message data in the packet.
     * @warning Invalid packets and non-status packets will be ignored by this function.
     */
    void Receive(const Packet& packet) 
    {
        if(not MotionControlProtocol::IsPacketValid(packet))
        {
            spdlog::debug("{}::{}() -> Received invalid packet! The packet was dropped.",CLASS_NAME,__func__);
            return;
        }

        const std::optional<MotionStatus> motion_status_opt = MotionControlProtocol::ExtractMotionStatus(packet);

        if(motion_status_opt.has_value())
        {
            m_motion_status_receiver_callback(motion_status_opt.value());
        }
    }

    /**
     * @brief Set a callback that StatusReceiver activates when a motion status message has been received.
     */
    void SetMotionStatusReceiverCallback(MotionStatusReceiverCallback callback) { m_motion_status_receiver_callback = std::move(callback); }

private:
    static constexpr std::string_view CLASS_NAME = "StatusReceiver";
    MotionStatusReceiverCallback m_motion_status_receiver_callback = [](const MotionStatus& status){(void)status;};
    
};
} // namespace RobotMiddleware