#pragma once
#include "motion_control_protocol.h"

#include <spdlog/spdlog.h>

#include <functional>
#include <string_view>

namespace RobotMiddleware
{

using MotionCommand = MotionControlProtocol::MotionCommand;

class CommandReceiver
{
public:
    // TODO: Generalize receiver callback logic to support other packet types
    using MotionCommandReceiverCallback = std::function<void(const MotionCommand& motion_command)>;

    CommandReceiver() = default;

    /**
     * @brief Receives a packet and conditionally extracts the command message data in the packet.
     * @warning Invalid packets and non-command packets will be ignored by this function.
     */
    void Receive(const Packet& packet) 
    {
        if(not MotionControlProtocol::IsPacketValid(packet))
        {
            spdlog::debug("{}::{}() -> Received invalid packet! The packet was dropped.",CLASS_NAME,__func__);
            return;
        }

        // TODO: Create a dispatcher function for identifying the command type and activating the appropriate RX callback

        const std::optional<MotionCommand> motion_command_opt = MotionControlProtocol::ExtractMotionCommand(packet);

        if(motion_command_opt.has_value())
        {
            m_motion_command_receiver_callback(motion_command_opt.value_or(MotionCommand{}));
        }
    }

    /**
     * @brief Set a callback that CommandReceiver activates when a motion command message has been received.
     */
    void SetMotionCommandReceiverCallback(MotionCommandReceiverCallback callback) { m_motion_command_receiver_callback = std::move(callback); }

private:

    static constexpr std::string_view CLASS_NAME = "CommandReceiver";

    MotionCommandReceiverCallback m_motion_command_receiver_callback = [](const MotionCommand& motion_command){ (void)motion_command; };
};

} // namespace RobotMiddleware