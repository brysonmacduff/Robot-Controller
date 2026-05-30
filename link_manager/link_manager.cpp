#include "link_manager.h"

namespace RobotController
{
LinkManager::LinkManager(uint8_t chip_enable_gpio, SpiOption spi_option, uint64_t radio_pipe_address, std::chrono::milliseconds radio_rx_timeout)
: m_radio_rx_timeout(radio_rx_timeout)
, m_transport_manager(MTP32::TransportManager(MTP32::Role::SLAVE
    , [&](MTP32::Packet tx_packet_bytes){ RequestRadioTx(tx_packet_bytes); }
    , [&](){ return RequestRadioRx(); }
    , [&](MTP32::Packet rx_packet_bytes){ HandleReceivedPacket(rx_packet_bytes); }
    , m_radio_rx_timeout))
    , m_radio(RF24(chip_enable_gpio, static_cast<uint8_t>(spi_option)))
{
    InitializeRadio(radio_pipe_address);
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

void LinkManager::InitializeRadio(uint64_t radio_pipe_address)
{
    // Initialize the radio hardware

    if (not m_radio.begin()) 
    {
        spdlog::critical("{}::{}() -> Failed to initialize radio hardware. This is a failure.",CLASS_NAME,__func__);
        return;
    }

    // These could be made configurable but it is unclear at this time whether they should be.
    m_radio.setPALevel(RF24_PA_LOW);
    m_radio.setDataRate(RF24_1MBPS);

    // Open pipes for both TX and RX
    m_radio.openWritingPipe(radio_pipe_address);
    m_radio.openReadingPipe(1, radio_pipe_address);
}

std::optional<MTP32::Packet> LinkManager::RequestRadioRx()
{
    // Poll the radio hardware to see if a packet was received
    if (not m_radio.available()) 
    {
        return std::nullopt;
    }

    // If a packet was received, return it.

    MTP32::Packet packet{};
    m_radio.read(packet.data(), packet.size());
    return packet;
}

void LinkManager::HandleReceivedPacket(const MTP32::Packet &rx_packet_bytes)
{
    RobotMiddleware::Packet rx_packet {};

    memcpy(&rx_packet,rx_packet_bytes.data(),sizeof(rx_packet));

    // Report the received packet to the listener
    m_rx_packet_callback(rx_packet);
}

void LinkManager::RequestRadioTx(const MTP32::Packet &tx_packet_bytes)
{
    // Order the radio hardware to transmit the packet
    
    m_radio.stopListening(); // Switch to TX mode
    m_radio.write(tx_packet_bytes.data(), tx_packet_bytes.size()); // Transmit the packet
    m_radio.startListening(); // Switch to RX mode
}

} // namespace RobotController