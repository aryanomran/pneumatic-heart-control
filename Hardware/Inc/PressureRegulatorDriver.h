/*
 * Created by Aryan Abbasgholitabaromran on 2025-10-30.
 */

#ifndef FIRMWARE_PRESSUREREGULATORDRIVER_H
#define FIRMWARE_PRESSUREREGULATORDRIVER_H

#pragma once

#include "Interfaces/PressureControl.h"
#include "Interfaces/AnalogSensor.h"
#include "Interfaces/AnalogActuator.h"

/**
 * @file PressureRegulatorDriver.h
 * @brief driver for the Festo VPPE.
 *
 * It implements the PressureControl interface. Its job is to contain
 * the high-level logic and calibration formulas to translate "Bar"
 * (from the App layer) into "Volts" (for the Peripherals layer).
 *
 * It USES the low-level peripherals.cpp to get the work done.
 */
class PressureRegulatorDriver : public PressureControl {
public:
    /**
     * @brief Constructs a new PressureRegulatorDriver.
     * @param setpointPin setting the output voltage (DAC).
     * @param feedbackPin reading the feedback voltage (ADC).
     */
    PressureRegulatorDriver(AnalogActuator& setpointPin, AnalogSensor& feedbackPin);

    /**
     * @brief Sets the target pressure.
     * Translates Bar -> Volts and tells the DAC.
     * @param bar The desired pressure in Bar.
     */
    bool setPressure(float bar) override;

    /**
     * @brief Reads the actual pressure.
     * Reads Volts from the ADC and translates Volts -> Bar.
     * @return The measured pressure in Bar.
     */
    float getActualPressure() override;

private:
    // These are the "Specialists" (Building Blocks) this driver uses.
    AnalogActuator& m_setpointPin; // Our "tool" to set the voltage
    AnalogSensor& m_feedbackPin;   // Our "tool" to read the voltage
};

#endif //FIRMWARE_PRESSUREREGULATORDRIVER_H