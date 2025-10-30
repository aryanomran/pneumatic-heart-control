//
// Created by Aryan Abbasgholitabaromran on 18/10/2025.
//

#ifndef FIRMWARE_IANALOGSENSOR_H
#define FIRMWARE_IANALOGSENSOR_H

#pragma once
/**
 * @file IAnalogSensor.h
 * @brief An abstract interface for any generic analog sensor.
 */

class IAnalogSensor {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IAnalogSensor() = default;
    /**
     * @brief Reads the raw voltage from the sensor.
     * @return The measured voltage
     */
    virtual float readVoltage() = 0;
};


#endif //FIRMWARE_IANALOGSENSOR_H
