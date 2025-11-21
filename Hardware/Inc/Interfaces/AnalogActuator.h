//
// Created by Aryan Abbasgholitabaromran on 22/10/2025.
//


#ifndef FIRMWARE_ANALOGACTUATOR_H
#define FIRMWARE_ANALOGACTUATOR_H

#pragma once
#include "../CommonTypes.h"

/**
 * @file AnalogActuator.h
 * @brief Abstract interface for a generic analog actuator.
 */

class AnalogActuator {
public:
    virtual ~AnalogActuator() = default;

    /**
     * @brief Initializes the actuator hardware (e.g. start DAC).
     */
    [[nodiscard]] virtual PeriphStatus init() = 0;

    /**
     * @brief Sets the output voltage.
     * @param voltage The desired voltage.
     * @return PeriphStatus::OK if successful.
     */
    [[nodiscard]] virtual PeriphStatus setVoltage(float voltage) = 0;
};

#endif //FIRMWARE_ANALOGACTUATOR_H