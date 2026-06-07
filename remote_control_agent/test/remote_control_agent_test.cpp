#include "remote_control_agent.h"
#include "motion_control_protocol.h"

#include <gtest/gtest.h>

#include <vector>

namespace 
{
using namespace RobotController;
using namespace RobotMiddleware;

class FakeLinkManager : public ILinkManager
{
public:
    void EnqueueTxPacket(const RobotMiddleware::Packet& packet) override { m_tx_packets.push_back(packet); };
    void SetRxPacketCallback(RxPacketCallback callback) override { m_rx_callback = std::move(callback); };
    void TriggerRxCallback(const RobotMiddleware::Packet& packet) { m_rx_callback(packet); }

    const std::vector<RobotMiddleware::Packet>& GetTxPackets () const { return m_tx_packets; }

private:
    std::vector<RobotMiddleware::Packet> m_tx_packets;
    RxPacketCallback m_rx_callback;
};

class FakeMotionController : public IMotionController
{
public:
    void ChangeMotionState(MotionState motion_state) override { m_motion_state = motion_state; };
    const MotionState& GetMotionState() const override { return m_motion_state; }

private:
    MotionState m_motion_state { MotionState::IDLE };
};

class RemoteControlAgentTest : public ::testing::Test
{
public:
    FakeMotionController fmc;
    FakeLinkManager flm;
};

/**
 * @brief This test verifies that the RemoteControlAgent reports the motion status of IMotionController to ILinkManager.
 * @note In a production context, this test is verifying behavioural logic for sending the motion status from the robot to the remote control device.
 * @result The test result is determined by comparing the IMotionController's initial state with the status received by ILinkManager.
 */
TEST_F(RemoteControlAgentTest, ReportMotionStatus)
{
    // The status reporting interval is set to zero so the RemoteControlAgent's state machine will instantly report the status when run.
    const std::chrono::milliseconds reporting_interval {0};

    RemoteControlAgent rma (flm, fmc, reporting_interval);

    ASSERT_TRUE(flm.GetTxPackets().empty());

    // Initialze the motion state
    fmc.ChangeMotionState(IMotionController::MotionState::ROTATE_CLOCKWISE);

    // Get the internal motion state for future test result assertion
    const MotionControlProtocol::MotionState initial_motion_state = MotionStateConvertor::LocalToMiddleware(fmc.GetMotionState());

    // Drives the RemoteControlAgent's state machine that is responsible for periodically reporting the IMotionController status.
    rma.Run(std::chrono::steady_clock::now());

    ASSERT_EQ(flm.GetTxPackets().size(), 1);
    
    const std::optional<MotionStatus> motion_status_opt = MotionControlProtocol::ExtractMotionStatus(flm.GetTxPackets()[0]);

    ASSERT_TRUE(motion_status_opt.has_value());
    EXPECT_EQ(motion_status_opt.value().motion_state, initial_motion_state);
}

/**
 * @brief This test verifies that the RemoteControlAgent reports a motion command from ILinkManager to IMotionController.
 * @note In a production context, this test verifies that the robot will process a motion command from the remote control device.
 */
TEST_F(RemoteControlAgentTest, ExecuteMotionCommand)
{
    const std::chrono::milliseconds reporting_interval {0};

    RemoteControlAgent rma (flm, fmc, reporting_interval);

    // Initialze the motion state
    fmc.ChangeMotionState(IMotionController::MotionState::IDLE);

    const Packet motion_command_packet = MotionControlProtocol::BuildMotionCommandPacket(MotionControlProtocol::MotionState::TRANSLATE_FORWARD, 42);

    // Make ILinkManager receive the motion command packet
    flm.TriggerRxCallback(motion_command_packet);

    // Verify that ILinkManager reported the motion command packet to RemoteControlAgent and the command was sent to IMotionController
    ASSERT_EQ(fmc.GetMotionState(), IMotionController::MotionState::TRANSLATE_FORWARD);
}

}