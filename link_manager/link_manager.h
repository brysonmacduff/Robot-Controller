#pragma once
#include "i_link_manager.h"
#include <mtp32/mtp32.h>
#include <chrono>
#include <cstring>

namespace RobotController
{
class LinkManager : public ILinkManager
{
public:
    LinkManager(std::chrono::milliseconds radio_rx_timeout = MTP32::TransportManager::RX_TIMEOUT);

    /**
     * @brief Schedule a packet to be transmitted at the next possible opportunity.
     */
    void EnqueueTxPacket(const RobotMiddleware::Packet& packet) override;
    void SetRxPacketCallback(RxPacketCallback callback) override;

    /**
     * @brief Drives the state machine behavior of the MTP32 RF communication protocol.
     */
    void Run(std::chrono::system_clock::time_point current_time);

private:
    RxPacketCallback m_rx_packet_callback;
    std::chrono::milliseconds m_radio_rx_timeout;
    MTP32::TransportManager m_transport_manager;

    void InitializeRadio();
    std::optional<MTP32::Packet> RequestRadioRx();
    void HandleReceivedPacket(const MTP32::Packet& rx_packet_bytes);
    void RequestRadioTx(const MTP32::Packet& tx_packet_bytes);
};
} // namespace RobotController