//
// Created by Aryan Abbasgholitabaromran on 28/10/2025.
//


#ifndef FIRMWARE_DIGITALACTUATOR_H
#define FIRMWARE_DIGITALACTUATOR_H

#pragma once
#include "../CommonTypes.h"

/**
 * @file DigitalActuator.h
 * @brief Abstract interface for a generic digital actuator (GPIO).
 */

class DigitalActuator {
public:
    virtual ~DigitalActuator() = default;

    /**
     * @brief Initializes the GPIO hardware.
     */
    [[nodiscard]] virtual PeriphStatus init() = 0;

    /**
     * @brief Writes a boolean state to the pin.
     * @param state true = High (3.3V), false = Low (GND).
     */
    virtual void write(bool state) = 0;

    /**
     * @brief Sets the output High.
     */
    virtual void setHigh() = 0;

    /**
     * @brief Sets the output Low.
     */
    virtual void setLow() = 0;

    /**
     * @brief Toggles the current state (High->Low or Low->High).
     */
    virtual void toggle() = 0;
};

#endif //FIRMWARE_DIGITALACTUATOR_H