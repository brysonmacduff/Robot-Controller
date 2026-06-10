#pragma once
#include <string_view>

namespace RobotController
{
/**
 * @brief This interface should be implemented by classes that are responsible for hardware diagnostics
 */
class IDiagnostic
{
public:
    virtual ~IDiagnostic() = default;

    virtual bool Initialize() = 0;
    virtual void Run() = 0;
    virtual bool GetResult() const = 0;
    virtual std::string_view GetName() const = 0;
};
};