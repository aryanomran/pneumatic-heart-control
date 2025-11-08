//
// Created by Aryan Abbasgholitabaromran on 20/10/2025.
//

#ifndef FIRMWARE_PRESSURESENSOR_H
#define FIRMWARE_PRESSURESENSOR_H

/**
 * @file PressureSensor.h
 * @brief Abstract interface for a pressure sensor.
 * A concrete class (MCLPressureSensor) ----> implement this.
 * That class will use an AnalogSensor in its constructor to get the
 * raw voltage, which it will then convert using its calibration formula.
 */

#pragma once

class PressureSensor {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~PressureSensor() = default;

    /**
     * @brief Reads the calibrated pressure.
     * @return The measured pressure in a standard unit (mmHg or Bar).
     */
    virtual float readPressure_mmHg() = 0;
};


#endif //FIRMWARE_PRESSURESENSOR_H