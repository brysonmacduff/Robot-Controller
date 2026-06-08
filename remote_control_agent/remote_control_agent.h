#pragma once
#include "i_link_manager.h"
#include "i_motion_controller.h"
#include "motion_control_protocol.h"
#include "status_sender.h"
#include "command_receiver.h"
#include "motion_state_convertor.h"

#include <spdlog/spdlog.h>

#include <chrono>
#include <string_view>

namespace RobotController
{
/**
 * @brief This class functions as the bridge between remote control command/status messages and the robot control logic they relate to.
 * @warning This class does take take resposibility for driving the state machine updates of the modules it holds references to. The main task loop should do this.
 */
class RemoteControlAgent
{
public:
    static constexpr std::chrono::milliseconds STATUS_REPORTING_INTERVAL {50};

    RemoteControlAgent(ILinkManager& link_manager, IMotionController& motion_controller, std::chrono::milliseconds status_reporting_interval = STATUS_REPORTING_INTERVAL);

    /**
     * @brief Drives the internal state machine. This is needed for things like reporting the robot status to the remote controller.
     */
    void Run(std::chrono::steady_clock::time_point current_time);

private:

    static constexpr std::string_view CLASS_NAME = "RemoteControlAgent";

    ILinkManager& m_link_manager;
    IMotionController& m_motion_controller;
    std::chrono::milliseconds m_status_reporting_interval;
    RobotMiddleware::CommandReceiver m_command_receiver;
    RobotMiddleware::StatusSender m_status_sender;
    std::chrono::steady_clock::time_point m_last_status_report_timepoint {};

    void SetLinkManagerRxCallback();
    void HandleRxPacket(const RobotMiddleware::Packet& packet);
    void InitializeMessageHandlingMiddleware();
    void ReportMotionStatus();
};
} // namespace RobotController