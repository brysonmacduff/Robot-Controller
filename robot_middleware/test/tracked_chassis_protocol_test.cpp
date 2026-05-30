#include "tracked_chassis_protocol.h"

#include <gtest/gtest.h>

namespace 
{
using namespace RobotMiddleware;

TEST(TrackedChassisProtocolTest, BuildMotionCommandPacket)
{
    const Packet packet = TrackedChassisProtocol::BuildMotionCommandPacket(TrackedChassisProtocol::MotionState::IDLE, 100);

    EXPECT_EQ(packet.type, Packet::COMMAND);
    EXPECT_EQ(packet.payload_size, 1);
    EXPECT_EQ(packet.payload_type, TrackedChassisProtocol::CommandType::ENACT_MOTION);
    EXPECT_EQ(packet.sequence_number, 100);
    EXPECT_EQ(packet.payload[0], TrackedChassisProtocol::MOTION_STATE_CODES.at(TrackedChassisProtocol::MotionState::IDLE));
}

TEST(TrackedChassisProtocolTest, BuildMotionStatusPacket)
{
    const Packet packet = TrackedChassisProtocol::BuildMotionStatusPacket(TrackedChassisProtocol::MotionState::IDLE, 100);

    EXPECT_EQ(packet.type, Packet::STATUS);
    EXPECT_EQ(packet.payload_size, 1);
    EXPECT_EQ(packet.payload_type, TrackedChassisProtocol::StatusType::MOTION_STATE);
    EXPECT_EQ(packet.sequence_number, 100);
    EXPECT_EQ(packet.payload[0], TrackedChassisProtocol::MOTION_STATE_CODES.at(TrackedChassisProtocol::MotionState::IDLE));
}

}