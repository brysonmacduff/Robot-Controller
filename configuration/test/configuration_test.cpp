#include "configuration.h"
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

namespace
{
using namespace RobotController;

TEST(ConfigurationTest, DefaultTrackChassisIsReturned)
{
    Configuration cfg;

    const auto& chassis = cfg.GetTrackChassis();

    EXPECT_EQ(chassis.left_track_motor_gpio, Configuration::DEFAULT_TRACK_CHASSIS.left_track_motor_gpio);
    EXPECT_EQ(chassis.right_track_motor_gpio, Configuration::DEFAULT_TRACK_CHASSIS.right_track_motor_gpio);
    EXPECT_TRUE(cfg.IsValid());
}

TEST(ConfigurationTest, ParseLongHelpArgument)
{
    Configuration cfg;

    const std::vector<std::string> arguments = {"--help",};

    EXPECT_TRUE(cfg.ParseArguments(arguments));
    EXPECT_FALSE(cfg.IsValid());
}

TEST(ConfigurationTest, ParseShortHelpArgument)
{
    Configuration cfg;

    const std::vector<std::string> arguments = {"-h",};

    EXPECT_TRUE(cfg.ParseArguments(arguments));
    EXPECT_FALSE(cfg.IsValid());
}

TEST(ConfigurationTest, ParseConfigFilePathLongArgument)
{
    Configuration cfg;
    const std::vector<std::string> arguments = {"--config-file-path", "config.yaml"};

    ASSERT_TRUE(cfg.ParseArguments(arguments));
    EXPECT_EQ(cfg.GetTrackChassis().left_track_motor_gpio, 10);
    EXPECT_EQ(cfg.GetTrackChassis().right_track_motor_gpio, 11);
    EXPECT_TRUE(cfg.IsValid());
}

TEST(ConfigurationTest, ParseConfigFilePathShortArgument)
{
    Configuration cfg;
    const char* raw_arguments[] = {"program", "-c", "config.yaml"};
    const int argument_count = 3;

    ASSERT_TRUE(cfg.ParseArguments(argument_count, raw_arguments));
    EXPECT_EQ(cfg.GetTrackChassis().left_track_motor_gpio, 10);
    EXPECT_EQ(cfg.GetTrackChassis().right_track_motor_gpio, 11);
    EXPECT_TRUE(cfg.IsValid());
}

TEST(ConfigurationTest, FailToParseConfigFileBecauseNoFileExists)
{
    Configuration cfg;
    const char* raw_arguments[] = {"program", "-c", "no_config.yaml"};
    const int argument_count = 3;

    ASSERT_FALSE(cfg.ParseArguments(argument_count, raw_arguments));
    // This is still considered a valid configuration because defaults will be used instead
    EXPECT_TRUE(cfg.IsValid());
}

TEST(ConfigurationTest, FailToParseConfigFileBecauseFileIsEmpty)
{
    Configuration cfg;
    const char* raw_arguments[] = {"program", "-c", "empty.yaml"};
    const int argument_count = 3;
    
    ASSERT_FALSE(cfg.ParseArguments(argument_count, raw_arguments));
    // This is still considered a valid configuration because defaults will be used instead
    EXPECT_TRUE(cfg.IsValid());
}

TEST(ConfigurationTest, FailToParseConfigFileBecauseWrongFileExtension)
{
    Configuration cfg;
    const char* raw_arguments[] = {"program", "-c", "wrong_extension.json"};
    const int argument_count = 3;
    
    ASSERT_FALSE(cfg.ParseArguments(argument_count, raw_arguments));
    // This is still considered a valid configuration because defaults will be used instead
    EXPECT_TRUE(cfg.IsValid());
}

TEST(ConfigurationTest, FailToParseConfigFileBecauseNoFilePathProvided)
{
    Configuration cfg;
    const char* raw_arguments[] = {"program", "-c"};
    const int argument_count = 2;
    
    ASSERT_FALSE(cfg.ParseArguments(argument_count, raw_arguments));
    // This is still considered a valid configuration because defaults will be used instead
    EXPECT_TRUE(cfg.IsValid());
}

} // namespace
