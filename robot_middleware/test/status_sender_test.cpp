#include "status_sender.h"

#include <gtest/gtest.h>

namespace 
{
using namespace RobotMiddleware;

TEST(StatusSenderTest, SendMotionStatus)
{
    const MotionStatus input_status { .motion_state = MotionControlProtocol::MotionState::IDLE, .sequence_number = 8 };

    bool is_callback_activated = false;

    StatusSender sender;

    sender.SetSenderCallback([&](const Packet& packet)
    {
        const std::optional<MotionStatus> motion_status_opt = MotionControlProtocol::ExtractMotionStatus(packet);
        
        is_callback_activated = true;

        ASSERT_TRUE(motion_status_opt.has_value());
        EXPECT_EQ(motion_status_opt.value().motion_state, input_status.motion_state);
        EXPECT_EQ(motion_status_opt.value().sequence_number, 0); // The first message sent should have a sequence number of zero
    });

    sender.Send(input_status);

    ASSERT_TRUE(is_callback_activated);
}

}