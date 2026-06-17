#include "status_receiver.h"

#include <gtest/gtest.h>

namespace 
{
using namespace RobotMiddleware;

TEST(StatusReceiverTest, ReceiveMotionStatus)
{
    const Packet input_packet = MotionControlProtocol::BuildMotionStatusPacket(MotionControlProtocol::MotionState::IDLE, 8);

    StatusReceiver receiver;

    bool is_callback_activated = false;

    receiver.SetMotionStatusReceiverCallback([&](const MotionStatus& status)
    {
        is_callback_activated = true;

        EXPECT_EQ(status.motion_state, MotionControlProtocol::MotionState::IDLE);
        EXPECT_EQ(status.sequence_number, 8);
    });

    receiver.Receive(input_packet);

    ASSERT_TRUE(is_callback_activated);
}
}