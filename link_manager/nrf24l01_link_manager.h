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
/**
 * @brief This class is a wrapper around the RF24 library which provides software control of the NRF24L01 bluetooth radio.
 * @note This class adheres to MTP32 for exchanging packets in point-to-point communication between radios.
 * @warning This class assumes that the NRF24L01 radio is connected via SPI and ready for use. Hardware initialization occurs in the constructor.
 */
class Nrf24l01LinkManager : public ILinkManager
{
public:

    static constexpr uint8_t DEFAULT_SPI_CE_GPIO = 22;
    static constexpr uint64_t DEFAULT_RADIO_PIPE_ADDRESS = 0xE8E8F0F0E1LL;

    enum class SpiOption
    {
        DEV0 = 0,
        DEV1 = 1
    };

    Nrf24l01LinkManager(uint8_t chip_enable_gpio = DEFAULT_SPI_CE_GPIO, 
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
     * @brief Initializes the radio.
     * @warning If the radio is not initialized, the TX packets will be dropped and no RX packets will be received.
     * @returns Whether the radio is initialized. 
     * @note Subsequent calls after a successful initialization will have no effect on the radio and just return true.
     */
    bool InitializeRadio();

    /**
     * @brief Drives the state machine behavior of the MTP32 RF communication protocol.
     * @note Used the current_time argument to determine the passage of time between Run() calls.
     */
    void Run(std::chrono::steady_clock::time_point current_time);

    bool IsRadioInitialized() const { return m_is_radio_initialized; }

private:

    static constexpr std::string_view CLASS_NAME = "LinkManager";

    RxPacketCallback m_rx_packet_callback;
    std::chrono::milliseconds m_radio_rx_timeout;
    MTP32::TransportManager m_transport_manager;
    uint64_t m_radio_pipe_address {0};
    RF24 m_radio;
    bool m_is_radio_initialized { false };

    /**
    * @brief Polls the radio hardware to check if a packet was received.
    * @returns The packet if available, std::nullopt if not.
    */
    std::optional<MTP32::Packet> RequestRadioRx();
    void HandleReceivedPacket(const MTP32::Packet& rx_packet_bytes);
    void RequestRadioTx(const MTP32::Packet& tx_packet_bytes);
};
} // namespace RobotController