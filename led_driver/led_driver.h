#pragma once
#include "i_led_driver.h"
#include <cstdint>
#include <memory>
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
     * @returns An LedDriver instance if successful, nullptr otherwise.
     */
    std::unique_ptr<LedDriver> Build(uint8_t led_gpio);

    /**
     * @brief Resets the LedDriver instance that is under construction.
     */
    void Reset();

private:

    std::unique_ptr<LedDriver> m_led_driver_ptr;
};

} // namespace RobotController