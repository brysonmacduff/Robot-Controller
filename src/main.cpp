#include "component_manager.h"

using namespace RobotController;

int main()
{
    ComponentManager component_manager;

    if(not component_manager.InitalizeComponents())
    {
        spdlog::error("Robot Controller is exiting due to a fatal error.");
        return 1;
    }

    spdlog::info("Robot Controller has started.");

    component_manager.StartTaskLoop();

    return 0;
}