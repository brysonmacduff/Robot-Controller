#include "nrf24l01_link_manager.h"

#include <gtest/gtest.h>

#include <thread>

/**
 * @brief The tests in this file depend on functional radio hardware to be connected with the raspberry pi hardware.
 * @warning This test will NOT work "off-board".
*/

namespace
{

using namespace RobotController;

TEST(Nrf24L01LinkManagerHardwareTest, InitializeRadio)
{
    Nrf24l01LinkManager link_manager = Nrf24l01LinkManager();

    EXPECT_TRUE(link_manager.InitializeRadio());

    EXPECT_TRUE(link_manager.IsRadioInitialized());

    // This should return true if the radio is already initialized
    EXPECT_TRUE(link_manager.InitializeRadio());
}

TEST(Nrf24L01LinkManagerHardwareTest, RunRxTxNopLoop)
{
    Nrf24l01LinkManager link_manager = Nrf24l01LinkManager();

    link_manager.SetRxPacketCallback([&](RobotMiddleware::Packet rx_packet)
    {
        const std::string rx_packet_str = rx_packet.ToString();
        spdlog::info("Nrf24l01LinkManager::RxCallback() -> RX: {}",rx_packet_str);
    });

    const uint32_t count_limit = 10;

    EXPECT_TRUE(link_manager.InitializeRadio());

    for(uint32_t counter = 0; counter < count_limit; ++counter)
    {
        auto current_time = std::chrono::steady_clock::now();
        link_manager.Run(current_time);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

}