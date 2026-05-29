#pragma once
#include "i_link_manager.h"
#include <mtp32/mtp32.h>
#include <list>

namespace RobotController
{
class LinkManager : public ILinkManager
{
public:
    void EnqueueTxPacket(const Packet& packet) override;
    void SetRxPacketCallback(RxPacketCallback callback) override;

    void Run();

private:
    RxPacketCallback m_rx_packet_callback;
    std::list<Packet> m_tx_queue;
};
} // namespace RobotController