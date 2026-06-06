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
    EXPECT_EQ(packet.payload_type, TrackedChassisProtocol::COMMAND_TYPE_CODES.at(TrackedChassisProtocol::CommandType::ENACT_MOTION));
    EXPECT_EQ(packet.sequence_number, 100);
    EXPECT_EQ(packet.payload[0], TrackedChassisProtocol::MOTION_STATE_CODES.at(TrackedChassisProtocol::MotionState::IDLE));
}

TEST(TrackedChassisProtocolTest, BuildMotionStatusPacket)
{
    const Packet packet = TrackedChassisProtocol::BuildMotionStatusPacket(TrackedChassisProtocol::MotionState::IDLE, 100);

    EXPECT_EQ(packet.type, Packet::STATUS);
    EXPECT_EQ(packet.payload_size, 1);
    EXPECT_EQ(packet.payload_type, TrackedChassisProtocol::STATUS_TYPE_CODES.at(TrackedChassisProtocol::StatusType::MOTION_STATE));
    EXPECT_EQ(packet.sequence_number, 100);
    EXPECT_EQ(packet.payload[0], TrackedChassisProtocol::MOTION_STATE_CODES.at(TrackedChassisProtocol::MotionState::IDLE));
}

TEST(TrackedChassisProtocolTest, ValidPacket)
{
    const Packet packet = TrackedChassisProtocol::BuildMotionStatusPacket(TrackedChassisProtocol::MotionState::IDLE, 100);

    ASSERT_TRUE(TrackedChassisProtocol::IsPacketValid(packet));
}

TEST(TrackedChassisProtocolTest, InvalidPacketType)
{
    Packet packet = TrackedChassisProtocol::BuildMotionStatusPacket(TrackedChassisProtocol::MotionState::IDLE, 100);
    packet.type = 0x02; // This is not a valid packet type (neither command nor status type)

    ASSERT_FALSE(TrackedChassisProtocol::IsPacketValid(packet));
}

TEST(TrackedChassisProtocolTest, InvalidPacketPayloadType)
{
    Packet packet = TrackedChassisProtocol::BuildMotionStatusPacket(TrackedChassisProtocol::MotionState::IDLE, 100);
    packet.payload_type = 0x05; // This is not a valid packet payload type (not a motion state status)

    ASSERT_FALSE(TrackedChassisProtocol::IsPacketValid(packet));
}

TEST(TrackedChassisProtocolTest, InvalidPacketPayloadSize)
{
    Packet packet = TrackedChassisProtocol::BuildMotionStatusPacket(TrackedChassisProtocol::MotionState::IDLE, 100);
    packet.payload_size = 100; // This is not a valid packet payload size (too big)

    ASSERT_FALSE(TrackedChassisProtocol::IsPacketValid(packet));
}

}