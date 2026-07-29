#pragma once
#include "frame_types.h"
#include <functional>

namespace RobotController
{

class IRangeSensorHub
{
public:
    using RangeSampleCallback = std::function<void(const EnuVector& range_sample_vector)>;

    virtual ~IRangeSensorHub() = default;

    virtual void SetRangeSampleCallback(RangeSampleCallback callback) = 0;
    virtual void Sample() = 0;
};
} // namespace RobotController