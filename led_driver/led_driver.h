#pragma once
#include "i_led_driver.h"
#include <cstdint>
#include <optional>
#include <pigpio.h>

namespace RobotController
{

// Forward declare factory class
class LedDriverFactory;

/**
 * @brief This class is a basic driver for control of an analog LED light.
 * @warning This class assumes the GPIO hardware is initialized (gpioInitialise() has already been called successfully).
 */
class LedDriver : public ILedDriver
{
public:
    bool Enable() override;
    bool Disable() override;

private:

    friend LedDriverFactory;

    uint8_t m_led_gpio { 0 };

    LedDriver(uint8_t led_gpio);
    bool Initialize();
};

/**
 * @brief This class is a factory for the LedDriver class.
 */
class LedDriverFactory
{
public:
    /**
     * @brief Attempts to build and initialize an LedDriver instance.
     * @returns An LedDriver instance if successful, std::nullopt otherwise.
     */
    std::optional<LedDriver> Build(uint8_t led_gpio);
};

} // namespace RobotController