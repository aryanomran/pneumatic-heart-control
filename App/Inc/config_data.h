//
// Created by Aryan Abbasgholitabaromran on 17/11/2025.
//
/*
 * This file defines a global, shared structure for
 * system-wide configuration and setpoints.
 */

#ifndef FIRMWARE_CONFIG_DATA_H
#define FIRMWARE_CONFIG_DATA_H

#include <stdint.h>

/**
 * @brief A structure to hold all configurable setpoints.
 *
 */
struct SystemConfig {
    // Pressure settings
    float systolePressure_bar  = 1.2f;

    // Changed default from 0.0f to 0.05f.
    float diastolePressure_bar = 0.05f;

    // Timing settings (for 60 BPM)
    uint32_t systoleDuration_ms  = 300;
    uint32_t diastoleDuration_ms = 700;
};

/**
 * @brief The single, global instance of the system configuration.
 *
 */
extern SystemConfig g_systemConfig;


#endif //FIRMWARE_CONFIG_DATA_H