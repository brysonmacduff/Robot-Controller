#include "component_manager.h"

namespace RobotController
{

ComponentManager::ComponentManager(std::chrono::milliseconds task_loop_period)
: m_task_loop_period(task_loop_period)
{
}

bool ComponentManager::InitalizeComponents()
{
    if(IsInitialized())
    {
        return true;
    }

    m_is_initialized = m_link_manager.InitializeRadio();

    m_link_manager.SetRxPacketCallback([&](RobotMiddleware::Packet rx_packet)
    {
        const std::string rx_packet_str = rx_packet.ToString();
        spdlog::info("Nrf24l01LinkManager::RxCallback() -> RX: {}",rx_packet_str);
    });

    return IsInitialized();
}

void ComponentManager::RunTaskLoop()
{
    if(not IsInitialized())
    {   
        spdlog::error("{}::{}() -> Failed to run task loop. Components are not initialized!",CLASS_NAME,__func__);
        return;
    }

    while(true)
    {
        auto current_time = std::chrono::steady_clock::now();
        
        m_link_manager.Run(current_time);

        std::this_thread::sleep_for(m_task_loop_period);
    }
}

} // namespace RobotController