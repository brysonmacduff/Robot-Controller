#include "component_manager.h"

using namespace RobotController;

int main(int argc, const char* argv[])
{
    Configuration configuration;
    
    configuration.ParseArguments(argc, argv);

    if(not configuration.IsValid())
    {
        return 1;
    }

    ComponentManager component_manager(configuration);

    if(not component_manager.InitalizeComponents())
    {
        spdlog::error("Robot Controller is exiting due to a fatal error.");
        return 1;
    }

    spdlog::info("Robot Controller has started.");

    component_manager.StartTaskLoop();

    return 0;
}