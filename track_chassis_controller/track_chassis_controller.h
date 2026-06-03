#pragma once
#include "i_track_chassis_controller.h"
#include "simple_motor_driver.h"

namespace RobotController
{
/**
 * @brief This class maintains the current motion state of the robot chassis amd models robot movement based on a tracked chassis design.
 * @note The robot chassis will enact the current motion state continuously.
 */
class TrackChassisController : ITrackChassisController
{
public:
    ~TrackChassisController();
    TrackChassisController(const TrackChassisController&) = delete;
    TrackChassisController& operator=(const TrackChassisController&) = delete;
    TrackChassisController(TrackChassisController&&) = delete;
    TrackChassisController&& operator=(TrackChassisController&&) = delete;

    void ChangeMotionState(MotionState motion_state) override;
    const MotionState& GetMotionState() const override { return m_motion_state; };

private:

    MotionState m_motion_state { MotionState::IDLE};

    SimpleMotorDriver m_right_track_motor;
    SimpleMotorDriver m_left_track_motor;

    void CeaseMotion();
    void RotateClockwise();
    void RotateAntiClockwise();
    void TranslateForward();
    void EnactMotionState();
};
} // namespace RobotController