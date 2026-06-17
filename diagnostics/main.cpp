#include "propulsion_diagnostic.h"

#include <pigpio.h>
#include <spdlog/spdlog.h>

using namespace RobotController;

int main()
{
    PropulsionDiagnostic propulsion_diagnostic(16, 12);

    if(propulsion_diagnostic.Initialize())
    {
        propulsion_diagnostic.Run();
        spdlog::info("{} : Diagnostic result = {}",propulsion_diagnostic.GetName(),propulsion_diagnostic.GetResult());
    }
    else
    {
        spdlog::error("{} : Failed initialization!",propulsion_diagnostic.GetName());
    }

    gpioTerminate();

    return 0;
}