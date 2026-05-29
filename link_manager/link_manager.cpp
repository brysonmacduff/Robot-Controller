#include "link_manager.h"

namespace RobotController
{
LinkManager::LinkManager(std::chrono::milliseconds radio_rx_timeout)
: m_radio_rx_timeout(radio_rx_timeout)
, m_transport_manager(MTP32::TransportManager(MTP32::Role::SLAVE
    , [&](MTP32::Packet tx_packet_bytes){ RequestRadioTx(tx_packet_bytes); }
    , [&](){ return RequestRadioRx(); }
    , [&](MTP32::Packet rx_packet_bytes){ HandleReceivedPacket(rx_packet_bytes); }
    , m_radio_rx_timeout))
{
    InitializeRadio();
}

void LinkManager::EnqueueTxPacket(const RobotMiddleware::Packet& packet)
{
    MTP32::Packet tx_packet_bytes {};

    memcpy(tx_packet_bytes.data(), &packet, MTP32::MAXIMUM_PACKET_SIZE);

    m_transport_manager.EnqueuePacket(tx_packet_bytes);
}

void LinkManager::SetRxPacketCallback(RxPacketCallback callback)
{
    m_rx_packet_callback = std::move(callback);
}

void LinkManager::Run(std::chrono::system_clock::time_point current_time)
{
    m_transport_manager.Run(current_time);
}

void LinkManager::InitializeRadio()
{
    // Initialize the radio hardware
}

std::optional<MTP32::Packet> LinkManager::RequestRadioRx()
{
    // Poll the radio hardware to see if a packet was received
    return std::nullopt;
}

void LinkManager::HandleReceivedPacket(const MTP32::Packet &rx_packet_bytes)
{
    RobotMiddleware::Packet rx_packet {};

    memcpy(&rx_packet,rx_packet_bytes.data(),sizeof(rx_packet));

    m_rx_packet_callback(rx_packet);
}

void LinkManager::RequestRadioTx(const MTP32::Packet &tx_packet_bytes)
{
    // Order the radio hardware to transmit the packet
}

} // namespace RobotController