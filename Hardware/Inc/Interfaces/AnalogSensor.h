//
// Created by Aryan Abbasgholitabaromran on 18/10/2025.
//

#ifndef FIRMWARE_ANALOGSENSOR_H
#define FIRMWARE_ANALOGSENSOR_H

#pragma once
#include "../CommonTypes.h"

/**
 * @file AnalogSensor.h
 * @brief An abstract interface for any generic analog sensor.
 */

class AnalogSensor {
public:
    virtual ~AnalogSensor() = default;

    /**
     * @brief Initializes the ADC/Sensor hardware.
     */
    [[nodiscard]] virtual PeriphStatus init() = 0;

    /**
     * @brief Reads the raw voltage from the sensor.
     * @param[out] voltage_out A reference to store the measured voltage.
     * @return PeriphStatus::OK if the reading is valid.
     */
    [[nodiscard]] virtual PeriphStatus readVoltage(float& voltage_out) = 0;
};

#endif //FIRMWARE_ANALOGSENSOR_H