//
// Created by Aryan Abbasgholitabaromran on 22/10/2025.
//

/**
 * @file Peripherals.cpp
 * @brief Implementation for the low-level classes.
 *
 * The only file to calls HAL functions (like HAL_ADC_GetValue, HAL_DAC_SetValue, HAL_GPIO_WritePin).
 * this is the layer who interact with the hardware by using HAL functions
 */

#include "Peripherals.h"

//               ANALOG INPUT (ADC)
/**
 * @brief Constructor: Stores the pointer to the ADC and the multiplier.
 */
STM32_AnalogIn::STM32_AnalogIn(ADC_HandleTypeDef* hadc, float voltage_multiplier)
        : m_hadc(hadc),        // Store the pointer to the ADC
          m_multiplier(voltage_multiplier) // Store the voltage divider ratio
{

}

/**
 * @brief Reads a single value from the ADC using the polling method.
 */
float STM32_AnalogIn::readVoltage()
{
    if (m_hadc == nullptr)
    {
        return -1.0f; // Safety check
    }

    // 1. Start the ADC conversion
    if (HAL_ADC_Start(m_hadc) != HAL_OK)
    {
        // Handle Start Error
        Error_Handler();
        return -1.0f;    // error value
    }

    // 2. Wait for the conversion to finish (with a 10ms timeout)
    if (HAL_ADC_PollForConversion(m_hadc, 10) != HAL_OK)
    {
        // Conversion failed or timed out
        Error_Handler();
        return -1.0f;
    }

    // 3. Get the 12-bit raw value (0-4095)
    uint32_t rawValue = HAL_ADC_GetValue(m_hadc);

    // 4. Stop the ADC

    HAL_ADC_Stop(m_hadc);

    // 5. Convert the raw 12-bit value to a 0-3.3V voltage
    float adcVoltage = ((float)rawValue / 4095.0f) * 3.3f;

    // 6. Apply the multiplier to get the real-world voltage
    return adcVoltage * m_multiplier;
}



//               ANALOG OUTPUT (DAC) IMPLEMENTATION

/**
 * @brief Constructor: Stores the DAC handle, channel, and starts the DAC.
 */
STM32_AnalogOut::STM32_AnalogOut(DAC_HandleTypeDef* hdac, uint32_t channel, float divider_ratio)
        : m_hdac(hdac),
          m_channel(channel),
          m_divider_ratio(divider_ratio)
{
    if (m_hdac == nullptr)
    {
        Error_Handler();
    }
    // Start the DAC peripheral.
    HAL_DAC_Start(m_hdac, m_channel);
}

/**
 * @brief Destructor: Stops the DAC.
 */
STM32_AnalogOut::~STM32_AnalogOut()
{
    if (m_hdac != nullptr)
    {
        HAL_DAC_Stop(m_hdac, m_channel);
    }
}

/**
 * @brief Sets the real-world output voltage.
 */
bool STM32_AnalogOut::setVoltage(float voltage)
{
    if (m_hdac == nullptr)
    {
        return false;
    }

    // 1. Scale the desired real-world voltage down to
    //    the DAC's 0-3.3V range.
    float dacVoltage = voltage / m_divider_ratio;

    // 2. saturate the voltage to the DAC's safe operating range (0V to 3.3V)
    if (dacVoltage < 0.0f) dacVoltage = 0.0f;
    if (dacVoltage > 3.3f) dacVoltage = 3.3f;

    // 3. Convert the 0-3.3V float value to a 12-bit raw integer (0-4095)

    uint32_t dacValue = (uint32_t)((dacVoltage / 3.3f) * 4095.0f);

    // 4. Set the DAC hardware value
    if (HAL_DAC_SetValue(m_hdac, m_channel, DAC_ALIGN_12B_R, dacValue) != HAL_OK)
    {
        Error_Handler();
        return false;
    }
    return true;
}



//               DIGITAL OUTPUT (GPIO) IMPLEMENTATION


/**
 * @brief Constructor: Stores the port/pin and sets the initial state.
 */
STM32_DigitalOut::STM32_DigitalOut(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState initialState)
        : m_port(port),
          m_pin(pin)
{
    if (m_port == nullptr)
    {
        Error_Handler();
    }
    // Set the initial state of the pin (I don't know valves is closed on boot or not)
    HAL_GPIO_WritePin(m_port, m_pin, initialState);
}

/**
 * @brief Sets the digital output to a high state (3.3V).
 */
void STM32_DigitalOut::setHigh()
{
    HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_SET);
}

/**
 * @brief Sets the digital output to a low state (0V / GND).
 */
void STM32_DigitalOut::setLow()
{
    HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_RESET);
}


