/*
 * Created by Aryan Abbasgholitabaromran on 2025-10-30.
 *
 * driver for the Festo VPPE.
 *
 * "translate" Bar <-> Volts.
 * It uses (IAnalogActuator, IAnalogSensor) to talk to the hardware.
 */

#include "PressureRegulatorDriver.h"


// inject the dependencies (low-level classes) when the object is created in main.cpp.
PressureRegulatorDriver::PressureRegulatorDriver(IAnalogActuator& setpointPin, IAnalogSensor& feedbackPin)
        : m_setpointPin(setpointPin),
          m_feedbackPin(feedbackPin)
{

    // setting the pressure to 0 when the system boots. (I need to ask this)
    setPressure(0.0f);
}


/**
 * @brief Sets the target pressure.
 * This function is the "translator" that converts Bar (from the App layer)
 * into Volts (for the Hardware layer).
 */
bool PressureRegulatorDriver::setPressure(float bar) {

    // the calibration formula:
    // Pressure = (V - 0.1) * (1.98 / 9.9) + 0.02
    //
    // reverse formula to solve for V (Voltage):
    // V = ((Pressure - 0.02) * (9.9 / 1.98)) + 0.1
    // V = ((Pressure - 0.02) * 5.0) + 0.1

    float voltage_to_set = ((bar - 0.02f) * 5.0f) + 0.1f;

    // to set that calculated voltage by DAC wrapper
    return m_setpointPin.setVoltage(voltage_to_set);
}


/**
 * @brief Reads the actual measured pressure.
 * that converts Volts (from the Hardware layer) into Bar (for the App layer).
 */
float PressureRegulatorDriver::getActualPressure() {

    // asking from (the ADC wrapper) to read the voltage.
    float feedback_voltage = m_feedbackPin.readVoltage();

    // Now, apply formula to convert Volts -> Bar
    // Pressure = (V - 0.1) * (1.98 / 9.9) + 0.02
    // Pressure = (V - 0.1) * 0.2 + 0.02

    float pressure_in_bar = ((feedback_voltage - 0.1f) * 0.2f) + 0.02f;
    // what if the voltage is 0?????
    // what formula must do?? probably return a small negative number ----> clamp it to 0.
    if (pressure_in_bar < 0.0f) {
        pressure_in_bar = 0.0f;
    }

    return pressure_in_bar;
}

