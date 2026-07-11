#include "configuration.h"

namespace RobotController
{

void Configuration::PrintHelpMessage()
{
    std::stringstream output;
    output << "*** Robot Controller Help Message ***\n\n";
    output << "> Configuration File Path <\n";
    output << "     --config-file-path <file path>\n";
    output << "     -c <file path>\n";
    output << "> Help <\n";
    output << "     --help\n";
    output << "     -h\n";
    output << "\n";
    output << "*** End of Help Message ***";

    std::cout << output.str() << std::endl;
}

bool Configuration::ParseArguments(const std::vector<std::string>& arguments)
{
    if(arguments.empty())
    {
        return false;
    }

    bool parsing_result = true;

    for(auto argument_it = arguments.begin(); argument_it != arguments.end(); ++argument_it)
    {
        if(*argument_it == "--help" || *argument_it == "-h")
        {   
            PrintHelpMessage();

            // The "help" command causes parsing to stop, so report that the configuration is not valid.
            m_is_valid = false;
            break;
        }
        else if(*argument_it == "--config-file-path" || *argument_it == "-c")
        {
            // Check if the following file path argument value exists
            if(argument_it + 1 == arguments.end())
            {
                parsing_result = false;
                spdlog::error("{}::{}() -> Failed to parse configuration file. No file path was provided.",CLASS_NAME,__func__);
                break;
            }

            // Increment past the argument flag to the argument value
            ++argument_it;

            if(not ParseConfigurationFromYamlFile(*argument_it))
            {
                parsing_result = false;
                spdlog::error("{}::{}() -> Failed to parse configuration file: <{}>.",CLASS_NAME,__func__,*argument_it);
                break;
            }
        }
    }

    if(not parsing_result)
    {
        spdlog::warn("{}::{}() -> Some parameters were not parsed correctly. Proceeding with default values accordingly.",CLASS_NAME,__func__);
    }

    return parsing_result;
}

bool Configuration::ParseArguments(int argument_count, const char* raw_arguments[])
{
    std::vector<std::string> arguments;
    arguments.reserve(argument_count);

    for(int count = 0; count < argument_count; ++count)
    {
        arguments.emplace_back(raw_arguments[count]);
    }

    return ParseArguments(arguments);
}

bool Configuration::IsYamlConfigFilePathValid(const std::string& file_path)
{
    if(file_path.empty())
    {
        return false;
    }

    std::filesystem::path path(file_path);

    // Must exist
    if (not std::filesystem::exists(path))
    {
        return false;
    }

    // Must be a regular file
    if (not std::filesystem::is_regular_file(path))
    {
        return false;
    }

    // Must have a YAML extension
    const auto ext = path.extension().string();
    if (ext != ".yaml" && ext != ".yml")
    {
        return false;
    }

    // Must be readable (best-effort check)
    std::ifstream file(file_path);
    if (not file.is_open())
    {
        return false;
    }

    return true;
}

bool Configuration::ParseConfigurationFromYamlFile(const std::string& file_path)
{
    if(not IsYamlConfigFilePathValid(file_path))
    {
        return false;
    }

    const YAML::Node config = YAML::LoadFile(file_path);

    if(not config["track_chassis"].IsDefined())
    {
        return false;
    }
    else if(not config["track_chassis"]["left_track_motor_gpio"].IsDefined())
    {
        return false;
    }
    else if(not config["track_chassis"]["right_track_motor_gpio"].IsDefined())
    {
        return false;
    }

    m_track_chassis.left_track_motor_gpio = config["track_chassis"]["left_track_motor_gpio"].as<uint8_t>();
    m_track_chassis.right_track_motor_gpio = config["track_chassis"]["right_track_motor_gpio"].as<uint8_t>();

    return true;
}

} // namespace RobotController