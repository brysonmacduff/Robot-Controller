#pragma once

namespace RobotController
{

/**
 * @brief IMotionController models the motion capabilities of a tracked chassis.
 */
class IMotionController
{
public:

    enum class MotionState
    {
        IDLE,
        TRANSLATE_FORWARD,
        ROTATE_CLOCKWISE,
        ROTATE_ANTICLOCKWISE
    };

    virtual ~IMotionController() = default;

    /**
     * @brief The implementor of this class is expected to execute the set motion state continuously.
     */
    virtual void ChangeMotionState(MotionState motion_state) = 0;
    /**
     * @brief Returns the current motion state that the implementor class should be enacting.
     */
    virtual const MotionState& GetMotionState() const = 0;
};
} // namespace RobotController