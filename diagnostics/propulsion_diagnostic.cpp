#include "propulsion_diagnostic.h"

namespace RobotController
{
PropulsionDiagnostic::PropulsionDiagnostic(uint8_t left_track_motor_gpio, uint8_t right_track_motor_gpio)
: m_left_track_motor(SimpleMotorDriver(left_track_motor_gpio))
, m_right_track_motor(SimpleMotorDriver(right_track_motor_gpio))
, m_track_chassis_controller(m_left_track_motor, m_right_track_motor)
{
}

bool PropulsionDiagnostic::Initialize() 
{
    return not (gpioInitialise() < 0);
}

void PropulsionDiagnostic::Run()
{
    m_result = false;

    m_track_chassis_controller.ChangeMotionState(IMotionController::MotionState::TRANSLATE_FORWARD);

    std::this_thread::sleep_for(DIAGNOSTIC_STEP_INTERVAL);

    m_track_chassis_controller.ChangeMotionState(IMotionController::MotionState::IDLE);

    std::this_thread::sleep_for(DIAGNOSTIC_STEP_INTERVAL);
    
    m_track_chassis_controller.ChangeMotionState(IMotionController::MotionState::ROTATE_CLOCKWISE);

    std::this_thread::sleep_for(DIAGNOSTIC_STEP_INTERVAL);

    m_track_chassis_controller.ChangeMotionState(IMotionController::MotionState::IDLE);

    std::this_thread::sleep_for(DIAGNOSTIC_STEP_INTERVAL);

    m_track_chassis_controller.ChangeMotionState(IMotionController::MotionState::ROTATE_ANTICLOCKWISE);

    std::this_thread::sleep_for(DIAGNOSTIC_STEP_INTERVAL);

    m_track_chassis_controller.ChangeMotionState(IMotionController::MotionState::IDLE);

    std::this_thread::sleep_for(DIAGNOSTIC_STEP_INTERVAL);

    m_result = true;
}

};