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


//           ANALOG INPUT (ADC)


/**
 * @brief Constructor: Stores the pointer to the ADC and the multiplier.
 */
STM32_AnalogIn::STM32_AnalogIn(ADC_HandleTypeDef* hadc, float voltage_multiplier)
        : m_hadc(hadc),        // Store the pointer to the ADC
          m_multiplier(voltage_multiplier) // Store the voltage divider ratio
{

}

/**
 * @brief Starts the ADC (if needed for continuous mode, etc.).
 * For polling, we often just check the handle validity.
 */
PeriphStatus STM32_AnalogIn::init()
{
    if (m_hadc == nullptr)
    {
        return PeriphStatus::ERROR_NULL_PTR;
    }

    return PeriphStatus::OK;
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




//           ANALOG OUTPUT (DAC)


/**
 * @brief Constructor: Stores the DAC handle, channel.
 */
STM32_AnalogOut::STM32_AnalogOut(DAC_HandleTypeDef* hdac, uint32_t channel, float divider_ratio)
        : m_hdac(hdac),
          m_channel(channel),
          m_divider_ratio(divider_ratio)
{

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
 * @brief Initializes the DAC peripheral.
 */
PeriphStatus STM32_AnalogOut::init()
{
    if (m_hdac == nullptr)
    {
        return PeriphStatus::ERROR_NULL_PTR;
    }

    // Start the DAC peripheral
    if (HAL_DAC_Start(m_hdac, m_channel) != HAL_OK)
    {
        return PeriphStatus::ERROR_HAL;
    }

    return PeriphStatus::OK;
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




//           DIGITAL OUTPUT (GPIO)



/**
 * @brief Constructor: Stores the port/pin and sets the initial state.
 */
STM32_DigitalOut::STM32_DigitalOut(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState initialState)
        : m_port(port),
          m_pin(pin)
{

    if (m_port != nullptr)
    {
        HAL_GPIO_WritePin(m_port, m_pin, initialState);
    }
}

/**
 * @brief Initializes the GPIO.
 */
PeriphStatus STM32_DigitalOut::init()
{
    if (m_port == nullptr)
    {
        return PeriphStatus::ERROR_NULL_PTR;
    }

    return PeriphStatus::OK;
}

/**
 * @brief Writes a boolean state to the pin.
 * @param state true = High (3.3V), false = Low (GND).
 */
void STM32_DigitalOut::write(bool state)
{
    if (state) {
        setHigh();
    } else {
        setLow();
    }
}

/**
 * @brief Sets the digital output to a high state (3.3V).
 */
void STM32_DigitalOut::setHigh()
{
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
    if (m_port != nullptr)
    {
        HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_RESET);
    }
}

/**
 * @brief Toggles the current state (High->Low or Low->High).
 */
void STM32_DigitalOut::toggle()
{
    if (m_port != nullptr)
    {
        HAL_GPIO_TogglePin(m_port, m_pin);
    }
}



//           SOLENOID VALVE


// Constructor
STM32_Solenoid::STM32_Solenoid(GPIO_TypeDef* port, uint16_t pin, bool activeLow)
        : m_port(port),
          m_pin(pin),
          m_activeLow(activeLow)
{
}

// Init: Ensure the valve starts in a safe (closed) state
PeriphStatus STM32_Solenoid::init() {
    if (m_port == nullptr) return PeriphStatus::ERROR_NULL_PTR;

    // Automatically close the valve on startup for safety
    deactivate();

    return PeriphStatus::OK;
}

// Activate: Energize the valve (Open it)
PeriphStatus STM32_Solenoid::activate() {
    if (m_port == nullptr) return PeriphStatus::ERROR_NULL_PTR;

    // Logic: If ActiveLow,  write RESET to Activate. Else  write SET.
    GPIO_PinState state = m_activeLow ? GPIO_PIN_RESET : GPIO_PIN_SET;

    HAL_GPIO_WritePin(m_port, m_pin, state);
    return PeriphStatus::OK;
}

// Deactivate: De-energize the valve (Close it)
PeriphStatus STM32_Solenoid::deactivate() {
    if (m_port == nullptr) return PeriphStatus::ERROR_NULL_PTR;

    // Logic: If ActiveLow,  write SET to Deactivate (Turn off). Else  write RESET.
    GPIO_PinState state = m_activeLow ? GPIO_PIN_SET : GPIO_PIN_RESET;

    HAL_GPIO_WritePin(m_port, m_pin, state);
    return PeriphStatus::OK;
}