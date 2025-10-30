//
// Created by Aryan Abbasgholitabaromran on 22/10/2025.
//

#ifndef FIRMWARE_IANALOGACTUATOR_H
#define FIRMWARE_IANALOGACTUATOR_H

/**
 * @file IAnalogActuator.h
 * @brief Abstract interface for a generic analog actuator.
 *
 * set a voltage.
 *
 * implemented by class: "Peripherals.cpp"
 * (wrapping the HAL_DAC) and used by a high-level driver (PressureRegulatorDriver).
 */

#pragma once

class IAnalogActuator {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IAnalogActuator() = default;

    /**
     * @brief Sets the output voltage.
     * @param voltage The desired voltage.
     */
    virtual bool setVoltage(float voltage) = 0;
};


#endif //FIRMWARE_IANALOGACTUATOR_H
