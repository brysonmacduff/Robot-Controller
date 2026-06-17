#include "command_receiver.h"

#include <gtest/gtest.h>

namespace 
{
using namespace RobotMiddleware;
TEST(CommandReceiverTest, ReceiveMotionCommand)
{
    const Packet input_packet = MotionControlProtocol::BuildMotionCommandPacket(MotionControlProtocol::MotionState::TRANSLATE_FORWARD, 8);

    CommandReceiver receiver;

    bool is_callback_activated = false;

    receiver.SetMotionCommandReceiverCallback([&](const MotionCommand& command)
    {
        is_callback_activated = true;

        EXPECT_EQ(command.motion_state, MotionControlProtocol::MotionState::TRANSLATE_FORWARD);
        EXPECT_EQ(command.sequence_number, 8);
    });

    receiver.Receive(input_packet);

    ASSERT_TRUE(is_callback_activated);
}
}