#pragma once
#include "nrf24l01_link_manager.h"

#include <spdlog/spdlog.h>

#include <chrono>
#include <thread>
#include <string_view>

namespace RobotController
{
class ComponentManager
{
public:
    static constexpr std::chrono::milliseconds DEFAULT_TASK_LOOP_PERIOD {50};

    ComponentManager(std::chrono::milliseconds task_loop_period = DEFAULT_TASK_LOOP_PERIOD);

    /**
     * @brief Initialize the components that this class manages.
     * @returns Whether all components were initialized.
     * @warning If this function returns false, it should be interpreted as a fatal error.
     * @note Subsequent calls after successful initialization will have no effect and just return true.
     */
    bool InitalizeComponents();

    /**
     * @brief Runs the tasks of the components that this class manages.
     * @note This function will block the calling thread.
     * @warning Running this function before components have been initialized will likely result in incorrect behaviour.
     */
    void RunTaskLoop();

    bool IsInitialized() const { return m_is_initialized; }

private:
    static constexpr std::string_view CLASS_NAME = "ComponentManager";

    std::chrono::milliseconds m_task_loop_period {DEFAULT_TASK_LOOP_PERIOD};
    Nrf24l01LinkManager m_link_manager;
    bool m_is_initialized { false };

};
} // namespace RobotController