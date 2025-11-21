//
// Created by Aryan Abbasgholitabaromran on 20/10/2025.
//

#ifndef FIRMWARE_PRESSURESENSOR_H
#define FIRMWARE_PRESSURESENSOR_H

#pragma once

// Include PeriphStatus'
#include "../CommonTypes.h"

/**
 * @file PressureSensor.h
 * @brief Abstract interface for a pressure sensor.
 * Returns a Status Code, passes data by reference.
 */

class PressureSensor {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~PressureSensor() = default;

    /**
     * @brief Initializes the sensor hardware (if needed).
     *  like Checks ID register, sets up I2C config, etc.
     * @return PeriphStatus::OK if initialization successful.
     */
    [[nodiscard]] virtual PeriphStatus init() = 0;

    /**
     * @brief Reads the calibrated pressure safely.
     * * @param[out] pressure_out A reference to store the result (in mmHg).
     * This value is only valid if the function returns OK.
     * @return PeriphStatus::OK if the reading was successful.
     * PeriphStatus::ERROR_TIMEOUT if sensor didn't respond.
     */
    [[nodiscard]] virtual PeriphStatus readPressure_mmHg(float& pressure_out) = 0;
};

#endif //FIRMWARE_PRESSURESENSOR_H