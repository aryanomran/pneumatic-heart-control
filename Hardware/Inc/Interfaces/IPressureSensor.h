//
// Created by Aryan Abbasgholitabaromran on 20/10/2025.
//

#ifndef FIRMWARE_IPRESSURESENSOR_H
#define FIRMWARE_IPRESSURESENSOR_H

/**
 * @file IPressureSensor.h
 * @brief Abstract interface for a pressure sensor.
 * A concrete class (MCLPressureSensor) ----> implement this.
 * That class will use an IAnalogSensor in its constructor to get the
 * raw voltage, which it will then convert using its calibration formula.
 */

#pragma once

class IPressureSensor {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IPressureSensor() = default;

    /**
     * @brief Reads the calibrated pressure.
     * @return The measured pressure in a standard unit (mmHg or Bar).
     */
    virtual float readPressure_mmHg() = 0;
};


#endif //FIRMWARE_IPRESSURESENSOR_H
