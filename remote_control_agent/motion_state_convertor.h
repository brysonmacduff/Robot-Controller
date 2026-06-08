#pragma once

#include "i_motion_controller.h"
#include "motion_control_protocol.h"

namespace RobotController
{
class MotionStateConvertor
{
public:
    MotionStateConvertor() = delete;

    static IMotionController::MotionState MiddlewareToLocal(const RobotMiddleware::MotionControlProtocol::MotionState& motion_state)
    {
        IMotionController::MotionState converted_motion_state {};

        switch (motion_state)
        {
            case RobotMiddleware::MotionControlProtocol::MotionState::TRANSLATE_FORWARD:
            {
                converted_motion_state = IMotionController::MotionState::TRANSLATE_FORWARD;
                break;
            }
            case RobotMiddleware::MotionControlProtocol::MotionState::ROTATE_CLOCKWISE:
            {
                converted_motion_state = IMotionController::MotionState::ROTATE_CLOCKWISE;
                break;
            }
            case RobotMiddleware::MotionControlProtocol::MotionState::ROTATE_ANTICLOCKWISE:
            {
                converted_motion_state = IMotionController::MotionState::ROTATE_ANTICLOCKWISE;
                break;
            }
            default:
            {
                converted_motion_state = IMotionController::MotionState::IDLE;
                break;
            }
        }

        return converted_motion_state;
    }

    static RobotMiddleware::MotionControlProtocol::MotionState LocalToMiddleware(const IMotionController::MotionState& motion_state)
    {
        RobotMiddleware::MotionControlProtocol::MotionState converted_motion_state {};
        
        switch (motion_state)
        {
            case IMotionController::MotionState::TRANSLATE_FORWARD:
            {
                converted_motion_state = RobotMiddleware::MotionControlProtocol::MotionState::TRANSLATE_FORWARD;
                break;
            }
            case IMotionController::MotionState::ROTATE_CLOCKWISE:
            {
                converted_motion_state = RobotMiddleware::MotionControlProtocol::MotionState::ROTATE_CLOCKWISE;
                break;
            }
            case IMotionController::MotionState::ROTATE_ANTICLOCKWISE:
            {
                converted_motion_state = RobotMiddleware::MotionControlProtocol::MotionState::ROTATE_ANTICLOCKWISE;
                break;
            }
            default:
            {
                converted_motion_state = RobotMiddleware::MotionControlProtocol::MotionState::IDLE;
                break;
            }
        }

        return converted_motion_state;
    }
};
} // namespace RobotController