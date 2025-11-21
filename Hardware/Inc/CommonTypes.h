//
// Created by Aryan Abbasgholitabaromran on 21/11/2025.
//

#ifndef FIRMWARE_COMMONTYPES_H
#define FIRMWARE_COMMONTYPES_H

#include <cstdint> // uint8_t

/**
 * @brief Standard status codes for all peripheral interactions.
 */
enum class PeriphStatus : uint8_t
{
    OK = 0,             // Operation succeeded
    ERROR_HAL,          // A generic HAL error occurred
    ERROR_TIMEOUT,      // The operation timed out
    ERROR_NULL_PTR,     // The peripheral handle was null
    ERROR_INVALID_STATE,// The peripheral was not initialized or is in the wrong state
    ERROR_OUT_OF_RANGE  // Input parameter was unsafe (e.g. > 2.0 Bar)
};

#endif //FIRMWARE_COMMONTYPES_H