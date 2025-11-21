//
// Created by Aryan Abbasgholitabaromran on 20/10/2025.
//



#ifndef FIRMWARE_SOLENOIDVALVE_H
#define FIRMWARE_SOLENOIDVALVE_H

#pragma once
#include "../CommonTypes.h"

/**
 * @file SolenoidValve.h
 * @brief Abstract interface for a generic solenoid valve.
 */

class SolenoidValve {
public:
    virtual ~SolenoidValve() = default;

    /**
     * @brief Initializes the valve hardware (GPIO configuration).
     */
    [[nodiscard]] virtual PeriphStatus init() = 0;

    /**
     * @brief Activates the valve (Energizes the coil).
     * For a Normally Closed (NC) valve, this Opens it.
     */
    virtual PeriphStatus activate() = 0;

    /**
     * @brief Deactivates the valve (De-energizes the coil).
     * For a Normally Closed (NC) valve, this Closes it.
     */
    virtual PeriphStatus deactivate() = 0;
};

#endif //FIRMWARE_SOLENOIDVALVE_H