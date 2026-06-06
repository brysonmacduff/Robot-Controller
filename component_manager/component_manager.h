#pragma once
#include "nrf24l01_link_manager.h"
#include "track_chassis_controller.h"
#include "remote_control_agent.h"
#include "simple_motor_driver.h"
#include "configuration.h"

#include <spdlog/spdlog.h>
#include <pigpio.h>

#include <chrono>
#include <thread>
#include <string_view>

namespace RobotController
{
class ComponentManager
{
public:
    static constexpr std::chrono::milliseconds DEFAULT_TASK_LOOP_PERIOD {50};

    ~ComponentManager();
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
    void StartTaskLoop();

    /**
     * @brief Stops the task loop.
     */
    void StopTaskLoop() { m_is_task_loop_enabled = false; }

    bool IsInitialized() const { return m_is_initialized; }

private:
    static constexpr std::string_view CLASS_NAME = "ComponentManager";

    std::chrono::milliseconds m_task_loop_period {DEFAULT_TASK_LOOP_PERIOD};
    Nrf24l01LinkManager m_link_manager;
    SimpleMotorDriver m_left_track_motor {Configuration::DEFAULT_TRACK_CHASSIS.left_track_motor_gpio};
    SimpleMotorDriver m_right_track_motor {Configuration::DEFAULT_TRACK_CHASSIS.right_track_motor_gpio};
    TrackChassisController m_track_chassis_controller;
    RemoteControlAgent m_remote_control_agent;
    bool m_is_initialized { false };
    bool m_is_task_loop_enabled { false };

};
} // namespace RobotController