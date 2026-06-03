#include "remote_control_protocol.h"

namespace RobotMiddleware
{
RemoteControlProtocol::SerializedPacket RemoteControlProtocol::Serialize(const Packet &packet)
{
    SerializedPacket packet_bytes;

    memcpy(packet_bytes.data(), &packet, sizeof(packet));

    return packet_bytes;
}

Packet RemoteControlProtocol::Deserialize(const SerializedPacket &bytes)
{
    Packet packet {};

    memcpy(&packet, bytes.data(), sizeof(SerializedPacket));

    return packet;
}

} // namespace RobotMiddleware