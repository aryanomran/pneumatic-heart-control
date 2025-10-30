//
// Created by Aryan Abbasgholitabaromran on 22/10/2025.
//

/**
 * @file Peripherals.h
 * @brief Header for the low-level classes.

 */

#ifndef FIRMWARE_PERIPHERALS_H
#define FIRMWARE_PERIPHERALS_H


#include "Interfaces/IAnalogSensor.h"
#include "Interfaces/IAnalogActuator.h"
#include "Interfaces/IDigitalActuator.h"


#include "main.h"
#include <stdint.h>


//               ANALOG INPUT (ADC)

/**
 * @class STM32_AnalogIn
 * @brief implementation  for the IAnalogSensor interface.
 *
 * job is to ----> how to interact to a single ADC peripheral to read a voltage.
 */
class STM32_AnalogIn : public IAnalogSensor {
public:
    /**
     * @brief Constructor.
     * @param hadc A pointer to the HAL ADC handle
     * @param voltage_multiplier A multiplier to convert 0-3.3V ADC reading
     * to the real-world voltage
     */
    STM32_AnalogIn(ADC_HandleTypeDef* hadc, float voltage_multiplier = 1.0f);
    virtual ~STM32_AnalogIn() = default;


    /**
     * @brief Reads the voltage from the ADC pin.
     * @return The measured voltage, scaled by the multiplier.
     */
    float readVoltage() override;

private:
    ADC_HandleTypeDef* m_hadc; // A pointer to the HAL ADC peripheral
    float m_multiplier;      // The scaling factor for our voltage divider
};


//               ANALOG OUTPUT (DAC)

/**
 * @class STM32_AnalogOut
 * @brief implementation for the IAnalogActuator interface.
 *
 * responsible to interact with the DAC peripheral to set a voltage.
 */
class STM32_AnalogOut : public IAnalogActuator {
public:
    /**
     * @brief Constructor.
     * @param hdac A pointer to the HAL DAC handle
     * @param channel The specific DAC channel
     * @param divider_ratio The voltage divider ratio.
     * Used to scale a high-level voltage (0-10V) down to the
     * DAC's 0-3.3V range.
     */
    STM32_AnalogOut(DAC_HandleTypeDef* hdac, uint32_t channel, float divider_ratio = 1.0f);
    virtual ~STM32_AnalogOut();


    /**
     * @brief Sets the output voltage.
     * @param voltage The desired real-world voltage.
     */
    bool setVoltage(float voltage) override;

private:
    DAC_HandleTypeDef* m_hdac;
    uint32_t m_channel;
    float m_divider_ratio;
};



//               DIGITAL OUTPUT (GPIO)

/**
 * @class STM32_DigitalOut
 * @brief implementation for the IDigitalActuator interface.
 *
 * talk to a GPIO pin to set it high or low.
 */
class STM32_DigitalOut : public IDigitalActuator {
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
     * @brief Sets the digital output to a high state (3.3V).
     */
    void setHigh() override;

    /**
     * @brief Sets the digital output to a low state (0V / GND).
     */
    void setLow() override;

private:
    GPIO_TypeDef* m_port; // The GPIO port
    uint16_t m_pin;       // The GPIO pin
};


#endif //FIRMWARE_PERIPHERALS_H

