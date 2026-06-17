#include "command_sender.h"

#include <gtest/gtest.h>

namespace 
{
using namespace RobotMiddleware;

TEST(CommandSenderTest, SendMotionCommand)
{
    const MotionCommand input_command { .motion_state = MotionControlProtocol::MotionState::IDLE, .sequence_number = 8 };

    bool is_callback_activated = false;

    CommandSender sender;

    sender.SetSenderCallback([&](const Packet& packet)
    {
        const std::optional<MotionCommand> motion_command_opt = MotionControlProtocol::ExtractMotionCommand(packet);
        
        is_callback_activated = true;

        ASSERT_TRUE(motion_command_opt.has_value());
        EXPECT_EQ(motion_command_opt.value().motion_state, input_command.motion_state);
        EXPECT_EQ(motion_command_opt.value().sequence_number, 0); // The first message sent should have a sequence number of zero
    });

    sender.Send(input_command);

    ASSERT_TRUE(is_callback_activated);
}

}