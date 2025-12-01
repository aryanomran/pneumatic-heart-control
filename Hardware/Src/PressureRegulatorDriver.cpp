/*
 * Created by Aryan Abbasgholitabaromran on 2025-10-30.
 */

#ifndef FIRMWARE_PRESSUREREGULATORDRIVER_H
#define FIRMWARE_PRESSUREREGULATORDRIVER_H

#pragma once

#include "Interfaces/PressureControl.h"
#include "Interfaces/AnalogSensor.h"
#include "Interfaces/AnalogActuator.h"
// RegulatorConfig is defined in CommonTypes.h, which is included by the interfaces above.

class PressureRegulatorDriver : public PressureControl {
public:
    /**
     * @brief Constructs a new PressureRegulatorDriver with specific limits.
     * * @param setpointPin The DAC interface to set voltage.
     * @param feedbackPin The ADC interface to read voltage.
     * @param config The struct containing Min/Max Pressure, Min/Max Voltage, and Safety Limits.
     * This replaces "magic numbers" and "isVacuum" flags.
     */
    PressureRegulatorDriver(AnalogActuator& setpointPin,
                            AnalogSensor& feedbackPin,
                            const RegulatorConfig& config);

    /**
     * @brief Sets the target pressure safely.
     * Uses the linear interpolation formula from the config struct.
     * Checks against config.safeMaxPressure before applying.
     */
    PeriphStatus setTargetPressure_Bar(float pressure_bar) override;

    /**
     * @brief Reads the actual pressure.
     * Converts Voltage -> Bar using the config struct.
     */
    PeriphStatus getActualPressure_Bar(float& pressure_out) const override;

    /**
     * @brief Initialize the driver and underlying hardware.
     */
    PeriphStatus init() override {
        if(m_setpointPin.init() != PeriphStatus::OK) return PeriphStatus::ERROR_HAL;
        if(m_feedbackPin.init() != PeriphStatus::OK) return PeriphStatus::ERROR_HAL;
        return PeriphStatus::OK;
    }

private:
    AnalogActuator& m_setpointPin;
    AnalogSensor& m_feedbackPin;

    // The configuration for this specific instance (Vacuum or Pressure)
    RegulatorConfig m_config;
};

#endif //FIRMWARE_PRESSUREREGULATORDRIVER_H