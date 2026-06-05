#include "track_chassis_controller.h"

#include <gtest/gtest.h>

namespace 
{
using namespace RobotController;
using MotionState = ITrackChassisController::MotionState;

struct FakeSimpleMotorDriver : public ISimpleMotorDriver
{
    bool is_active { false };

    bool ActivateMotor() override { is_active = true; return is_active; }
    bool DeactivateMotor() override { is_active = false; return is_active; }
    bool IsMotorActive() const override { return is_active; }
};

/**
 * @brief Test fixture for Track Chassis Controller unit tests
 */
class TrackChassisControllerTest : public ::testing::Test
{
public:
    ISimpleMotorDriver& GetLeftMotor() { return m_left_motor; }
    ISimpleMotorDriver& GetRightMotor() { return m_right_motor; }
private:
    FakeSimpleMotorDriver m_left_motor;
    FakeSimpleMotorDriver m_right_motor;
};

TEST_F(TrackChassisControllerTest, SetIdleMotionState)
{
    ISimpleMotorDriver& left_motor = GetLeftMotor();
    ISimpleMotorDriver& right_motor = GetRightMotor();

    TrackChassisController tcc (left_motor, right_motor);

    tcc.ChangeMotionState(MotionState::IDLE);

    EXPECT_EQ(tcc.GetMotionState(),MotionState::IDLE);
    EXPECT_FALSE(left_motor.IsMotorActive());
    EXPECT_FALSE(right_motor.IsMotorActive());
}

TEST_F(TrackChassisControllerTest, TranslateForward)
{
    ISimpleMotorDriver& left_motor = GetLeftMotor();
    ISimpleMotorDriver& right_motor = GetRightMotor();

    TrackChassisController tcc (left_motor, right_motor);

    tcc.ChangeMotionState(MotionState::TRANSLATE_FORWARD);

    EXPECT_EQ(tcc.GetMotionState(),MotionState::TRANSLATE_FORWARD);
    EXPECT_TRUE(left_motor.IsMotorActive());
    EXPECT_TRUE(right_motor.IsMotorActive());
}

TEST_F(TrackChassisControllerTest, RotateClockwise)
{
    ISimpleMotorDriver& left_motor = GetLeftMotor();
    ISimpleMotorDriver& right_motor = GetRightMotor();

    TrackChassisController tcc (left_motor, right_motor);

    tcc.ChangeMotionState(MotionState::ROTATE_CLOCKWISE);
    EXPECT_EQ(tcc.GetMotionState(),MotionState::ROTATE_CLOCKWISE);
    EXPECT_TRUE(left_motor.IsMotorActive());
    EXPECT_FALSE(right_motor.IsMotorActive());
}

TEST_F(TrackChassisControllerTest, RotateAntiClockwise)
{
    ISimpleMotorDriver& left_motor = GetLeftMotor();
    ISimpleMotorDriver& right_motor = GetRightMotor();

    TrackChassisController tcc (left_motor, right_motor);

    tcc.ChangeMotionState(MotionState::ROTATE_ANTICLOCKWISE);
    EXPECT_EQ(tcc.GetMotionState(),MotionState::ROTATE_ANTICLOCKWISE);
    EXPECT_FALSE(left_motor.IsMotorActive());
    EXPECT_TRUE(right_motor.IsMotorActive());
}

}