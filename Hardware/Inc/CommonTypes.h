//
// Created by Aryan Abbasgholitabaromran on 21/11/2025.
//

#ifndef FIRMWARE_COMMONTYPES_H
#define FIRMWARE_COMMONTYPES_H

#include <cstdint>

/**
 * @brief Defines the status codes for the peripheral driver layer.
 * Shared across all interfaces and drivers.
 */
enum class PeriphStatus : uint8_t
{
    OK = 0,             // Operation succeeded
    ERROR_HAL,          // A generic HAL error occurred
    ERROR_TIMEOUT,      // The operation timed out
    ERROR_NULL_PTR,     // The peripheral handle was null
    ERROR_INVALID_STATE, // The peripheral was not initialized or in a bad state
    ERROR_OUT_OF_RANGE   // The requested value (pressure) is unsafe
};

/**
 * @brief Configuration struct to define the physical limits of a regulator.
 * This removes "magic numbers" from the driver code.
 */
struct RegulatorConfig {
    float minPressureBar;  // Physical minimum (e.g. 0.0 for Standard, -1.0 for Vacuum)
    float maxPressureBar;  // Physical maximum (e.g. 6.0 for Standard, 0.0 for Vacuum)

    float minVoltage;      // Voltage at minPressure (e.g. 0.1V or 0.0V)
    float maxVoltage;      // Voltage at maxPressure (e.g. 10.0V)

    float safeMaxPressure; // Software Safety Limit (e.g. 5.5 bar). Rejects commands above this.
};

#endif //FIRMWARE_COMMONTYPES_H