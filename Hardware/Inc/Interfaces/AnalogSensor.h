//
// Created by Aryan Abbasgholitabaromran on 18/10/2025.
//

#ifndef FIRMWARE_ANALOGSENSOR_H
#define FIRMWARE_ANALOGSENSOR_H

#pragma once
#include <cstdint>

/**
 * @file AnalogSensor.h
 * @brief An abstract interface for any generic analog sensor.
 */


enum class PeriphStatus : uint8_t;

class AnalogSensor {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~AnalogSensor() = default;

    /**
     * @brief Reads the raw voltage from the sensor.
     * @param[out] voltage_out A reference to store the measured voltage.
     * @return A PeriphStatus code indicating success or failure.
     */

    virtual PeriphStatus readVoltage(float& voltage_out) = 0;
};


#endif //FIRMWARE_ANALOGSENSOR_H