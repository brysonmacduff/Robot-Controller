#include "link_manager.h"

namespace RobotController
{
void LinkManager::EnqueueTxPacket(const Packet &packet)
{
    m_tx_queue.push_back(packet);
}

void LinkManager::SetRxPacketCallback(RxPacketCallback callback)
{
    m_rx_packet_callback = std::move(callback);
}

} // namespace RobotController