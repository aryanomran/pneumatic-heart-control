/*
 * Created by Aryan Abbasgholitabaromran on 2025-10-30.
 *
 * driver for the Festo VPPE.
 *
 * "translate" Bar <-> Volts.
 * It uses (AnalogActuator, AnalogSensor) to talk to the hardware.
 */

#include "PressureRegulatorDriver.h"
#include "Peripherals.h"

// inject the dependencies (low-level classes) when the object is created in main.cpp.

PressureRegulatorDriver::PressureRegulatorDriver(AnalogActuator& setpointPin, AnalogSensor& feedbackPin)
        : m_setpointPin(setpointPin),
          m_feedbackPin(feedbackPin)
{

    // setting the pressure to 0 when the system boots.
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
    PeriphStatus status = m_setpointPin.setVoltage(voltage_to_set);

    // Convert the PeriphStatus to the 'bool' this function must return.
    return (status == PeriphStatus::OK);
}


/**
 * @brief Reads the actual measured pressure.
 * that converts Volts (from the Hardware layer) into Bar (for the App layer).
 */
float PressureRegulatorDriver::getActualPressure() {

    // 1. Declare a variable to hold the voltage.
    float feedback_voltage = 0.0f;

    // 2. Pass that variable by reference to readVoltage.
    // The function will fill it with the measured value.
    PeriphStatus status = m_feedbackPin.readVoltage(feedback_voltage);

    // 3. Check if the read failed.
    if (status != PeriphStatus::OK)
    {
        // If we couldn't read the voltage, return 0.0 as a safe value.
        return 0.0f;
    }

    // 4. Now, 'feedback_voltage' holds the value,
    //    and we can apply the formula.
    float pressure_in_bar = ((feedback_voltage - 0.1f) * 0.2f) + 0.02f;

    if (pressure_in_bar < 0.0f) {
        pressure_in_bar = 0.0f;
    }

    return pressure_in_bar;
}