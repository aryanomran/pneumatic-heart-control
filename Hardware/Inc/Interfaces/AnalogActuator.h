//
// Created by Aryan Abbasgholitabaromran on 22/10/2025.
//

#ifndef FIRMWARE_ANALOGACTUATOR_H
#define FIRMWARE_ANALOGACTUATOR_H

/**
 * @file AnalogActuator.h
 * @brief Abstract interface for a generic analog actuator.
 */

#pragma once
#include <cstdint>


enum class PeriphStatus : uint8_t;

class AnalogActuator {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~AnalogActuator() = default;

    /**
     * @brief Sets the output voltage.
     * @param voltage The desired voltage.
     * @return A PeriphStatus code indicating success or failure.
     */

    virtual PeriphStatus setVoltage(float voltage) = 0;
};


#endif //FIRMWARE_ANALOGACTUATOR_H