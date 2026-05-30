#pragma once
#include "i_link_manager.h"

#include <mtp32/mtp32.h>
#include <RF24/RF24.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <cstring>
#include <string_view>

namespace RobotController
{

class LinkManager : public ILinkManager
{
public:

    static constexpr uint8_t DEFAULT_SPI_CE_GPIO = 22;
    static constexpr uint64_t DEFAULT_RADIO_PIPE_ADDRESS = 0xE8E8F0F0E1LL;

    enum class SpiOption
    {
        DEV0 = 0,
        DEV1 = 1
    };

    LinkManager(uint8_t chip_enable_gpio = DEFAULT_SPI_CE_GPIO, 
        SpiOption spi_option = SpiOption::DEV0,
        uint64_t radio_pipe_address = DEFAULT_RADIO_PIPE_ADDRESS,
        std::chrono::milliseconds radio_rx_timeout = MTP32::TransportManager::RX_TIMEOUT
    );

    /**
     * @brief Schedule a packet to be transmitted at the next possible opportunity.
     */
    void EnqueueTxPacket(const RobotMiddleware::Packet& packet) override;
    void SetRxPacketCallback(RxPacketCallback callback) override;

    /**
     * @brief Drives the state machine behavior of the MTP32 RF communication protocol.
     * @note Used the current_time argument to determine the passage of time between Run() calls.
     */
    void Run(std::chrono::system_clock::time_point current_time);

private:

    static constexpr std::string_view CLASS_NAME = "LinkManager";

    RxPacketCallback m_rx_packet_callback;
    std::chrono::milliseconds m_radio_rx_timeout;
    MTP32::TransportManager m_transport_manager;
    RF24 m_radio;

    void InitializeRadio(uint64_t radio_pipe_address);
    /**
    * @brief Polls the radio hardware to check if a packet was received.
    * @returns The packet if available, std::nullopt if not.
    */
    std::optional<MTP32::Packet> RequestRadioRx();
    void HandleReceivedPacket(const MTP32::Packet& rx_packet_bytes);
    void RequestRadioTx(const MTP32::Packet& tx_packet_bytes);
};
} // namespace RobotController