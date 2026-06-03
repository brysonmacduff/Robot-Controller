#include "track_chassis_controller.h"

namespace RobotController
{
TrackChassisController::~TrackChassisController()
{
}

TrackChassisController::TrackChassisController(ISimpleMotorDriver& left_track_motor, ISimpleMotorDriver& right_track_motor)
: m_left_track_motor(left_track_motor)
, m_right_track_motor(right_track_motor)
{
}

void TrackChassisController::ChangeMotionState(MotionState motion_state)
{
    // Only react if the new motion state is different than the current motion state.
    if(motion_state != m_motion_state)
    {
        m_motion_state = motion_state;
        EnactMotionState();
    }
}

void TrackChassisController::CeaseMotion()
{
    m_left_track_motor.DeactivateMotor();
    m_right_track_motor.DeactivateMotor();
}

void TrackChassisController::RotateClockwise()
{
    m_left_track_motor.ActivateMotor();
    m_right_track_motor.DeactivateMotor();
}

void TrackChassisController::RotateAntiClockwise()
{
    m_left_track_motor.DeactivateMotor();
    m_right_track_motor.ActivateMotor();
}

void TrackChassisController::TranslateForward()
{
    m_left_track_motor.ActivateMotor();
    m_right_track_motor.ActivateMotor();
}

void TrackChassisController::EnactMotionState()
{
    switch (m_motion_state)
    {
    case MotionState::TRANSLATE_FORWARD:
        TranslateForward();
        break;
    case MotionState::ROTATE_CLOCKWISE:
        RotateClockwise();
        break;
    case MotionState::ROTATE_ANTICLOCKWISE:
        RotateAntiClockwise();
        break;
    default:
        CeaseMotion();
        break;
    }
}
} // namespace RobotController