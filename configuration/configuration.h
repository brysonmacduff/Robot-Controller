#pragma once
#include <cstdint>
#include <sstream>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

namespace RobotController
{

class Configuration
{
public:

    struct TrackChassis
    {
        uint8_t left_track_motor_gpio {0};
        uint8_t right_track_motor_gpio {0};
    };

    inline static const TrackChassis DEFAULT_TRACK_CHASSIS { 16, 12 };

    static constexpr uint8_t DEFAULT_STARTUP_LED_GPIO { 26 };

    Configuration() = default;

    /**
     * @brief Parses raw CLI arguments from the main function. If successful, the parsed configuration will be stored in this class.
     * @returns Whether parsing was successful. However, this does not indicate whether the configuration is valid.
    */
    bool ParseArguments(int argument_count, const char* raw_arguments[]);

    /**
     * @brief Parses CLI arguments. If successful, the parsed configuration will be stored in this class.
     * @returns Whether parsing was successful. However, this does not indicate whether the configuration is valid.
    */
    bool ParseArguments(const std::vector<std::string>& arguments);

    const TrackChassis& GetTrackChassis() const { return m_track_chassis; }
    uint8_t GetStartupLedGpio() const { return m_startup_led_gpio; }

    /**
     * @brief Indicates whether the configuration is in a valid state. In some cases, the configuration can be parsed from invalid instructions.
    */
    bool IsValid() const { return m_is_valid; }

private:

    static constexpr std::string_view CLASS_NAME = "Configuration";

    bool m_is_valid { true };
    TrackChassis m_track_chassis { DEFAULT_TRACK_CHASSIS };
    uint8_t m_startup_led_gpio { DEFAULT_STARTUP_LED_GPIO };

    bool ParseConfigurationFromYamlFile(const std::string& file_path);
    bool IsYamlConfigFilePathValid(const std::string& file_path);
    void PrintHelpMessage();
};
} // namespace RobotController