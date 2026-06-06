#include "motion_control_protocol.h"

#include <gtest/gtest.h>

namespace 
{
using namespace RobotMiddleware;

TEST(MotionControlProtocolTest, BuildMotionCommandPacket)
{
    const Packet packet = MotionControlProtocol::BuildMotionCommandPacket(MotionControlProtocol::MotionState::IDLE, 100);

    EXPECT_EQ(packet.type, Packet::COMMAND);
    EXPECT_EQ(packet.payload_size, 1);
    EXPECT_EQ(packet.payload_type, MotionControlProtocol::COMMAND_TYPE_CODES.at(MotionControlProtocol::CommandType::ENACT_MOTION));
    EXPECT_EQ(packet.sequence_number, 100);
    EXPECT_EQ(packet.payload[0], MotionControlProtocol::MOTION_STATE_CODES.at(MotionControlProtocol::MotionState::IDLE));
}

TEST(MotionControlProtocolTest, BuildMotionStatusPacket)
{
    const Packet packet = MotionControlProtocol::BuildMotionStatusPacket(MotionControlProtocol::MotionState::IDLE, 100);

    EXPECT_EQ(packet.type, Packet::STATUS);
    EXPECT_EQ(packet.payload_size, 1);
    EXPECT_EQ(packet.payload_type, MotionControlProtocol::STATUS_TYPE_CODES.at(MotionControlProtocol::StatusType::MOTION_STATE));
    EXPECT_EQ(packet.sequence_number, 100);
    EXPECT_EQ(packet.payload[0], MotionControlProtocol::MOTION_STATE_CODES.at(MotionControlProtocol::MotionState::IDLE));
}

TEST(MotionControlProtocolTest, ValidPacket)
{
    const Packet packet = MotionControlProtocol::BuildMotionStatusPacket(MotionControlProtocol::MotionState::IDLE, 100);

    ASSERT_TRUE(MotionControlProtocol::IsPacketValid(packet));
}

TEST(MotionControlProtocolTest, InvalidPacketType)
{
    Packet packet = MotionControlProtocol::BuildMotionStatusPacket(MotionControlProtocol::MotionState::IDLE, 100);
    packet.type = 0x02; // This is not a valid packet type (neither command nor status type)

    ASSERT_FALSE(MotionControlProtocol::IsPacketValid(packet));
}

TEST(MotionControlProtocolTest, InvalidPacketPayloadType)
{
    Packet packet = MotionControlProtocol::BuildMotionStatusPacket(MotionControlProtocol::MotionState::IDLE, 100);
    packet.payload_type = 0x05; // This is not a valid packet payload type (not a motion state status)

    ASSERT_FALSE(MotionControlProtocol::IsPacketValid(packet));
}

TEST(MotionControlProtocolTest, InvalidPacketPayloadSize)
{
    Packet packet = MotionControlProtocol::BuildMotionStatusPacket(MotionControlProtocol::MotionState::IDLE, 100);
    packet.payload_size = 100; // This is not a valid packet payload size (too big)

    ASSERT_FALSE(MotionControlProtocol::IsPacketValid(packet));
}

}