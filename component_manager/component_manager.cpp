#include "component_manager.h"

namespace RobotController
{

ComponentManager::~ComponentManager()
{
    gpioTerminate();
}

ComponentManager::ComponentManager(std::chrono::milliseconds task_loop_period)
: m_task_loop_period(task_loop_period)
, m_track_chassis_controller(m_left_track_motor, m_right_track_motor)
, m_remote_control_agent(m_link_manager, m_track_chassis_controller)
{
}

bool ComponentManager::InitalizeComponents()
{
    if(IsInitialized())
    {
        return true;
    }

    if(gpioInitialise() < 0)
    {
        spdlog::error("{}::{}() -> Failed to initialize GPIO hardward!",CLASS_NAME,__func__);
        return false;
    }

    m_is_initialized = m_link_manager.InitializeRadio();

    return IsInitialized();
}

void ComponentManager::StartTaskLoop()
{
    if(not IsInitialized())
    {   
        spdlog::error("{}::{}() -> Failed to run task loop. Components are not initialized!",CLASS_NAME,__func__);
        return;
    }

    m_is_task_loop_enabled = true;

    while(m_is_task_loop_enabled)
    {
        auto current_time = std::chrono::steady_clock::now();
        
        m_link_manager.Run(current_time);

        current_time = std::chrono::steady_clock::now();

        m_remote_control_agent.Run(current_time);

        std::this_thread::sleep_for(m_task_loop_period);
    }
}

} // namespace RobotController