#pragma once
#include "remote_control_protocol.h"
#include <functional>

namespace RobotController
{
class ILinkManager
{
public:
    using Packet = RobotMiddleware::Packet;

    using RxPacketCallback = std::function<void(const Packet& packet)>;

    virtual ~ILinkManager() = default;

    virtual void EnqueueTxPacket(const Packet& packet) = 0;
    virtual void SetRxPacketCallback(RxPacketCallback callback) = 0;

};
} // namespace RobotController