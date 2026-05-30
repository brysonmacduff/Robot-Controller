#include "remote_control_protocol.h"
#include <gtest/gtest.h>

namespace 
{
using namespace RobotMiddleware;

TEST(RemoteControlProtocolTest, SerializePacket)
{
    Packet packet {};
    packet.type = 0x08;
    packet.sequence_number = 0xFF;
    packet.payload_type = 0x24;
    packet.payload_size = 0x01;
    packet.payload[0] = 0x48;

    const RemoteControlProtocol::SerializedPacket serialized_packet = RemoteControlProtocol::Serialize(packet);

    EXPECT_EQ(serialized_packet[0], packet.type);
    EXPECT_EQ(serialized_packet[1], packet.sequence_number);
    EXPECT_EQ(serialized_packet[2], packet.payload_type);
    EXPECT_EQ(serialized_packet[3], packet.payload_size);
    EXPECT_EQ(serialized_packet[4], packet.payload[0]);
}

TEST(RemoteControlProtocolTest, DeserializePacket)
{
    RemoteControlProtocol::SerializedPacket serialized_packet = {0};
    serialized_packet[0] = 0x08;
    serialized_packet[1] = 0xFF;
    serialized_packet[2] = 0x24;
    serialized_packet[3] = 0x01;
    serialized_packet[4] = 0x48;

    const Packet packet = RemoteControlProtocol::Deserialize(serialized_packet);
    
    EXPECT_EQ(serialized_packet[0], packet.type);
    EXPECT_EQ(serialized_packet[1], packet.sequence_number);
    EXPECT_EQ(serialized_packet[2], packet.payload_type);
    EXPECT_EQ(serialized_packet[3], packet.payload_size);
    EXPECT_EQ(serialized_packet[4], packet.payload[0]);
}
}