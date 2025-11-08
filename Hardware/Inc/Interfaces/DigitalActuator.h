//
// Created by Aryan Abbasgholitabaromran on 28/10/2025.
//

#ifndef FIRMWARE_DIGITALACTUATOR_H
#define FIRMWARE_DIGITALACTUATOR_H

/**
 * @file DigitalActuator.h
 * @brief Abstract interface for a generic digital actuator (on/off).
 *
 * does one job any simple digital output: set its state.
 *
 * implemented in "Peripherals.cpp"
 * (wrapping HAL_GPIO_WritePin) and used by a high-level driver (ISolenoidValve)
 */

#pragma once

class DigitalActuator {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~DigitalActuator() = default;

    /**
     * @brief Sets the digital output to a high state (3.3V).
     */
    virtual void setHigh() = 0;

    /**
     * @brief Sets the digital output to a low state (0V / GND).
     */
    virtual void setLow() = 0;
};


#endif //FIRMWARE_DIGITALACTUATOR_H
