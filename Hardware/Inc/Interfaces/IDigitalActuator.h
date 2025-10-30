//
// Created by Aryan Abbasgholitabaromran on 28/10/2025.
//

#ifndef FIRMWARE_IDIGITALACTUATOR_H
#define FIRMWARE_IDIGITALACTUATOR_H

/**
 * @file IDigitalActuator.h
 * @brief Abstract interface for a generic digital actuator (on/off).
 *
 * does one job any simple digital output: set its state.
 *
 * implemented in "Peripherals.cpp"
 * (wrapping HAL_GPIO_WritePin) and used by a high-level driver (ISolenoidValve)
 */

#pragma once

class IDigitalActuator {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IDigitalActuator() = default;

    /**
     * @brief Sets the digital output to a high state (3.3V).
     */
    virtual void setHigh() = 0;

    /**
     * @brief Sets the digital output to a low state (0V / GND).
     */
    virtual void setLow() = 0;
};


#endif //FIRMWARE_IDIGITALACTUATOR_H
