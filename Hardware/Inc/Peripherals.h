//
// Created by Aryan Abbasgholitabaromran on 22/10/2025.
//

/**
 * @file Peripherals.h
 * @brief Header for the low-level classes.
 */

#ifndef FIRMWARE_PERIPHERALS_H
#define FIRMWARE_PERIPHERALS_H

#include "Interfaces/AnalogSensor.h"
#include "Interfaces/AnalogActuator.h"
#include "Interfaces/DigitalActuator.h"
#include "Interfaces/SolenoidValve.h"

#include "main.h"
#include <stdint.h>


//           ANALOG INPUT (ADC)


/**
 * @class STM32_AnalogIn
 * @brief implementation for the AnalogSensor interface.
 */
class STM32_AnalogIn : public AnalogSensor {
public:
    /**
     * @brief Constructor.
     * @param hadc A pointer to the HAL ADC handle
     * @param voltage_multiplier A multiplier to convert 0-3.3V ADC reading to real-world voltage.
     */
    STM32_AnalogIn(ADC_HandleTypeDef* hadc, float voltage_multiplier = 1.0f);
    virtual ~STM32_AnalogIn() = default;

    /**
     * @brief Starts the ADC peripheral.
     */
    PeriphStatus init() override;

    /**
     * @brief Reads the voltage from the ADC pin.
     */
    PeriphStatus readVoltage(float& voltage_out) override;

private:
    ADC_HandleTypeDef* m_hadc;
    float m_multiplier;
};



//           ANALOG OUTPUT (DAC)


/**
 * @class STM32_AnalogOut
 * @brief implementation for the AnalogActuator interface.
 */
class STM32_AnalogOut : public AnalogActuator {
public:
    /**
     * @brief Constructor.
     * @param hdac A pointer to the HAL DAC handle
     * @param channel The specific DAC channel
     * @param divider_ratio Used to scale high-level voltage down to DAC range.
     */
    STM32_AnalogOut(DAC_HandleTypeDef* hdac, uint32_t channel, float divider_ratio = 1.0f);
    virtual ~STM32_AnalogOut();

    /**
     * @brief Starts the DAC peripheral.
     */
    PeriphStatus init() override;

    /**
     * @brief Sets the output voltage.
     */
    PeriphStatus setVoltage(float voltage) override;

private:
    DAC_HandleTypeDef* m_hdac;
    uint32_t m_channel;
    float m_divider_ratio;
};



//           DIGITAL OUTPUT (GPIO)


/**
 * @class STM32_DigitalOut
 * @brief implementation for the DigitalActuator interface.
 */
class STM32_DigitalOut : public DigitalActuator {
public:
    /**
     * @brief Constructor.
     * @param port The GPIO port
     * @param pin The GPIO pin
     * @param initialState The state the pin should be in at startup.
     */
    STM32_DigitalOut(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState initialState = GPIO_PIN_RESET);
    virtual ~STM32_DigitalOut() = default;

    /**
     * @brief Initializes the GPIO (Optional check).
     */
    PeriphStatus init() override;

    /**
     * @brief Writes a specific boolean state.
     */
    void write(bool state) override;

    /**
     * @brief Sets the digital output to a high state (3.3V).
     */
    void setHigh() override;

    /**
     * @brief Sets the digital output to a low state (0V / GND).
     */
    void setLow() override;

    /**
     * @brief Toggles the pin state.
     */
    void toggle() override;

private:
    GPIO_TypeDef* m_port;
    uint16_t m_pin;
};



//           SOLENOID VALVE


/**
 * @class STM32_Solenoid
 * @brief Implementation for the SolenoidValve interface.
 * Controls a GPIO pin to open/close a valve using HAL directly.
 */
class STM32_Solenoid : public SolenoidValve {
public:
    /**
     * @brief Constructor
     * @param port GPIO Port (GPIOA)
     * @param pin GPIO Pin (GPIO_PIN_5)
     * @param activeLow If true, logic LOW opens the valve. If false, logic HIGH opens it.
     */
    STM32_Solenoid(GPIO_TypeDef* port, uint16_t pin, bool activeLow = false);
    virtual ~STM32_Solenoid() = default;

    // Interface Implementation
    PeriphStatus init() override;
    PeriphStatus activate() override;   // Opens valve
    PeriphStatus deactivate() override; // Closes valve

private:
    GPIO_TypeDef* m_port;
    uint16_t m_pin;
    bool m_activeLow;
};

#endif //FIRMWARE_PERIPHERALS_H