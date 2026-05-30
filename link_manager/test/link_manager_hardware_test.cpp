#include "link_manager.h"

#include <gtest/gtest.h>

#include <thread>
#include <ostream>

/**
 * @brief The tests in this file depend on functional radio hardware to be connected with the raspberry pi hardware.
 * @warning This test will NOT work "off-board".
*/

namespace
{

using namespace RobotController;

TEST(LinkManagerHardwareTest, InitializeRadio)
{
    ASSERT_NO_FATAL_FAILURE(LinkManager());
}

TEST(LinkManagerHardwareTest, RunRxTxNopLoop)
{
    LinkManager link_manager = LinkManager();

    link_manager.SetRxPacketCallback([&](RobotMiddleware::Packet rx_packet)
    {
        std::cout << "Received packet: " << rx_packet.ToString() << "\n";
    });

    const uint8_t count_limit = 16;

    for(uint8_t counter = 0; counter < count_limit; ++counter)
    {
        auto current_time = std::chrono::system_clock::now();
        link_manager.Run(current_time);
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
}

}