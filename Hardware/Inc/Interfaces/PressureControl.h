//
// Created by Aryan Abbasgholitabaromran on 18/10/2025.
//


#ifndef FIRMWARE_PRESSURECONTROL_H
#define FIRMWARE_PRESSURECONTROL_H

#pragma once
#include "../CommonTypes.h"

/**
 * @file PressureControl.h
 * @brief Abstract interface for a pressure control system (Regulator).
 */

class PressureControl {
public:
    virtual ~PressureControl() = default;

    /**
     * @brief Initializes the regulator hardware.
     */
    [[nodiscard]] virtual PeriphStatus init() = 0;

    /**
     * @brief Sets the target pressure safely.
     * @param pressure_bar The desired pressure in Bar.
     * @return PeriphStatus::OK if the command was accepted.
     * Returns ERROR_OUT_OF_RANGE if pressure_bar > MaxSafeLimit.
     */
    [[nodiscard]] virtual PeriphStatus setTargetPressure_Bar(float pressure_bar) = 0;

    /**
     * @brief Reads the actual measured pressure.
     * @param[out] pressure_out Reference to store the result.
     * @return PeriphStatus::OK if reading is valid.
     */
    [[nodiscard]] virtual PeriphStatus getActualPressure_Bar(float& pressure_out) const = 0;
};

#endif //FIRMWARE_PRESSURECONTROL_H