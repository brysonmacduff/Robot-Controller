#include "remote_control_agent.h"

namespace RobotController
{
RemoteControlAgent::RemoteControlAgent(ILinkManager &link_manager, IMotionController &motion_controller, std::chrono::milliseconds status_reporting_interval)
: m_link_manager(link_manager)
, m_motion_controller(motion_controller)
, m_status_reporting_interval(status_reporting_interval)
{
    SetLinkManagerRxCallback();
    InitializeMessageHandlingMiddleware();
}

void RemoteControlAgent::Run(std::chrono::steady_clock::time_point current_time)
{
    if(current_time < m_last_status_report_timepoint + m_status_reporting_interval)
    {
        return;
    }

    ReportMotionStatus();

    m_last_status_report_timepoint = current_time;
}

void RemoteControlAgent::ReportMotionStatus()
{
    const IMotionController::MotionState motion_state = m_motion_controller.GetMotionState();

    RobotMiddleware::MotionStatus motion_status {};
    motion_status.motion_state = MotionStateConvertor::LocalToMiddleware(motion_state);

    m_status_sender.Send(motion_status);
}

void RemoteControlAgent::SetLinkManagerRxCallback()
{
    m_link_manager.SetRxPacketCallback([&](RobotMiddleware::Packet rx_packet)
    {
        if(not RobotMiddleware::MotionControlProtocol::IsPacketValid(rx_packet))
        {
            spdlog::warn("{}::LinkManagerRxCallback() -> Dropped invalid packet.", CLASS_NAME);
            return;
        }

        spdlog::debug("LinkManager::RxCallback() -> RX: {}",rx_packet.ToString());

        HandleRxPacket(rx_packet);
    });
}

void RemoteControlAgent::HandleRxPacket(const RobotMiddleware::Packet& packet)
{
    m_command_receiver.Receive(packet);
}

void RemoteControlAgent::InitializeMessageHandlingMiddleware()
{
    /**
     * Currently there is only one type of command message (motion state command). However, in the future the command receiver will need to support additional
     * command message types (add more callbacks perhaps for each command type).
    */ 
    m_command_receiver.SetMotionCommandReceiverCallback([&](const RobotMiddleware::MotionCommand& motion_command)
    { 
        m_motion_controller.ChangeMotionState(MotionStateConvertor::MiddlewareToLocal(motion_command.motion_state));
    });

    // Simply forward the outbound packet to Link Manager
    m_status_sender.SetSenderCallback([&](const RobotMiddleware::Packet& packet) 
    {
        m_link_manager.EnqueueTxPacket(packet);
    });
}

} // namespace RobotController