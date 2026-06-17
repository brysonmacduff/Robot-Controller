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
    const Packet packet = MotionControlProtocol::BuildMotionStatusPacket(MotionControlProtocol::MotionState::TRANSLATE_FORWARD, 100);

    EXPECT_EQ(packet.type, Packet::STATUS);
    EXPECT_EQ(packet.payload_size, 1);
    EXPECT_EQ(packet.payload_type, MotionControlProtocol::STATUS_TYPE_CODES.at(MotionControlProtocol::StatusType::MOTION_STATE));
    EXPECT_EQ(packet.sequence_number, 100);
    EXPECT_EQ(packet.payload[0], MotionControlProtocol::MOTION_STATE_CODES.at(MotionControlProtocol::MotionState::TRANSLATE_FORWARD));
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

TEST(MotionControlProtocolTest, IsMotionCommandPacket)
{
    Packet packet {};
    packet.type = Packet::COMMAND;
    packet.payload_type = MotionControlProtocol::COMMAND_TYPE_CODES.at(MotionControlProtocol::CommandType::ENACT_MOTION);
    packet.payload[0] = MotionControlProtocol::MOTION_STATE_CODES.at(MotionControlProtocol::MotionState::TRANSLATE_FORWARD);
    packet.payload_size = 1;
    packet.sequence_number = 22;
    
    EXPECT_TRUE(MotionControlProtocol::IsMotionCommandPacket(packet));
}

TEST(MotionControlProtocolTest, IsNotMotionCommandPacket)
{
    Packet packet {};
    packet.type = Packet::STATUS;
    
    EXPECT_FALSE(MotionControlProtocol::IsMotionCommandPacket(packet));
}

TEST(MotionControlProtocolTest, IsMotionStatusPacket)
{
    Packet packet {};
    packet.type = Packet::STATUS;
    packet.payload_type = MotionControlProtocol::STATUS_TYPE_CODES.at(MotionControlProtocol::StatusType::MOTION_STATE);
    packet.payload[0] = MotionControlProtocol::MOTION_STATE_CODES.at(MotionControlProtocol::MotionState::TRANSLATE_FORWARD);
    packet.payload_size = 1;
    packet.sequence_number = 22;
    
    EXPECT_TRUE(MotionControlProtocol::IsMotionStatusPacket(packet));
}

TEST(MotionControlProtocolTest, IsNotMotionStatusPacket)
{
    Packet packet {};
    packet.type = Packet::COMMAND;
    
    EXPECT_FALSE(MotionControlProtocol::IsMotionStatusPacket(packet));
}

TEST(MotionControlProtocolTest, ExtractMotionCommand)
{
    Packet packet {};
    packet.type = Packet::COMMAND;
    packet.payload_type = MotionControlProtocol::COMMAND_TYPE_CODES.at(MotionControlProtocol::CommandType::ENACT_MOTION);
    packet.payload[0] = MotionControlProtocol::MOTION_STATE_CODES.at(MotionControlProtocol::MotionState::TRANSLATE_FORWARD);
    packet.payload_size = 1;
    packet.sequence_number = 22;
    
    const auto motion_command_opt = MotionControlProtocol::ExtractMotionCommand(packet);

    ASSERT_TRUE(motion_command_opt.has_value());
    EXPECT_EQ(motion_command_opt.value().motion_state, MotionControlProtocol::MotionState::TRANSLATE_FORWARD);
    EXPECT_EQ(motion_command_opt.value().sequence_number, packet.sequence_number);
}

TEST(MotionControlProtocolTest, ExtractMotionStatus)
{
    Packet packet {};
    packet.type = Packet::STATUS;
    packet.payload_type = MotionControlProtocol::STATUS_TYPE_CODES.at(MotionControlProtocol::StatusType::MOTION_STATE);
    packet.payload[0] = MotionControlProtocol::MOTION_STATE_CODES.at(MotionControlProtocol::MotionState::TRANSLATE_FORWARD);
    packet.payload_size = 1;
    packet.sequence_number = 21;

    const auto motion_status_opt = MotionControlProtocol::ExtractMotionStatus(packet);
    
    ASSERT_TRUE(motion_status_opt.has_value());
    EXPECT_EQ(motion_status_opt.value().motion_state, MotionControlProtocol::MotionState::TRANSLATE_FORWARD);
    EXPECT_EQ(motion_status_opt.value().sequence_number, packet.sequence_number);
}

}