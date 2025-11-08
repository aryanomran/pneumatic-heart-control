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
    // Constructor no longer calls Error_Handler().
    // A null pointer will be safely caught by readVoltage().
}

/**
 * @brief Reads a single value from the ADC using the polling method.
 * @param[out] voltage_out A reference to store the measured voltage.
 * @return A PeriphStatus code indicating success or failure.
 */
PeriphStatus STM32_AnalogIn::readVoltage(float& voltage_out)
{
    if (m_hadc == nullptr)
    {
        return PeriphStatus::ERROR_NULL_PTR; // Safety check
    }

    // 1. Start the ADC conversion
    if (HAL_ADC_Start(m_hadc) != HAL_OK)
    {
        // Handle Start Error - Report, don't halt.
        return PeriphStatus::ERROR_HAL;
    }

    // 2. Wait for the conversion to finish (with a 10ms timeout)
    if (HAL_ADC_PollForConversion(m_hadc, 10) != HAL_OK)
    {
        // Conversion failed or timed out - Report, don't halt.
        HAL_ADC_Stop(m_hadc); // Attempt to stop ADC anyway
        return PeriphStatus::ERROR_TIMEOUT;
    }

    // 3. Get the 12-bit raw value (0-4095)
    uint32_t rawValue = HAL_ADC_GetValue(m_hadc);

    // 4. Stop the ADC
    HAL_ADC_Stop(m_hadc);

    // 5. Convert the raw 12-bit value to a 0-3.3V voltage
    float adcVoltage = ((float)rawValue / 4095.0f) * 3.3f;

    // 6. Apply the multiplier, store in the out-parameter, and return OK
    voltage_out = adcVoltage * m_multiplier;
    return PeriphStatus::OK;
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
        // Error_Handler() removed. The null pointer will be
        // safely caught by setVoltage() and returned as an error.
    }
    else
    {
        // Start the DAC peripheral only if the handle is valid.
        HAL_DAC_Start(m_hdac, m_channel);
    }
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
 * @return A PeriphStatus code indicating success or failure.
 */
PeriphStatus STM32_AnalogOut::setVoltage(float voltage)
{
    if (m_hdac == nullptr)
    {
        return PeriphStatus::ERROR_NULL_PTR;
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
        // Report HAL error, don't halt
        return PeriphStatus::ERROR_HAL;
    }

    // Return success
    return PeriphStatus::OK;
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
    { // <-- THE 'D:' WAS REMOVED FROM HERE
        // Error_Handler() removed. Null pointer will be
        // safely ignored by setHigh/setLow.
    }
    else
    {
        // Set the initial state of the pin only if the port is valid
        HAL_GPIO_WritePin(m_port, m_pin, initialState);
    }
}

/**
 * @brief Sets the digital output to a high state (3.3V).
 */
void STM32_DigitalOut::setHigh()
{
    // Add null check to prevent crash if constructor failed
    if (m_port != nullptr)
    {
        HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_SET);
    }
}

/**
 * @brief Sets the digital output to a low state (0V / GND).
 */
void STM32_DigitalOut::setLow()
{
    // Add null check to prevent crash if constructor failed
    if (m_port != nullptr)
    {
        HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_RESET);
    }
}