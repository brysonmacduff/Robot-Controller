#pragma once

#include "i_diagnostic.h"
#include "track_chassis_controller.h"
#include "simple_motor_driver.h"

#include <pigpio.h>

#include <chrono>
#include <thread>

namespace RobotController
{
/**
 * @brief This class is reponsible for hardware diagnostics that affect the robot propulsion system.
 */
class PropulsionDiagnostic : public IDiagnostic
{
public:
    PropulsionDiagnostic(uint8_t left_track_motor_gpio, uint8_t right_track_motor_gpio);

    bool Initialize() override;
    void Run() override;
    bool GetResult() const override { return m_result; }
    std::string_view GetName() const override { return CLASS_NAME; }

private:
    static constexpr std::string_view CLASS_NAME = "PropulsionDiagnostic";
    static constexpr std::chrono::milliseconds DIAGNOSTIC_STEP_INTERVAL {1000};

    SimpleMotorDriver m_left_track_motor;
    SimpleMotorDriver m_right_track_motor;
    TrackChassisController m_track_chassis_controller;
    bool m_result { false };
};
};