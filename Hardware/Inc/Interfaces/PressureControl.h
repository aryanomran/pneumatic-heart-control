//
// Created by Aryan Abbasgholitabaromran on 18/10/2025.
//

#ifndef FIRMWARE_PRESSURECONTROL_H
#define FIRMWARE_PRESSURECONTROL_H

/**
 * @file PressureControl.h
 * @brief Abstract interface for a pressure control system.
 *
 * This is the high-level interface for the RTOS tasks (in the App layer)
 * will use to command the pressure regulator.
 *
 * It hides all the implementation details (like the Festo VPPE) from the application.
 */

#pragma once

class PressureControl {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~PressureControl() = default;

    /**
     * @brief Sets the target pressure for the regulator.
     * @param bar The desired pressure in Bar (0.0 to 2.0).
     */
    virtual bool setPressure(float bar) = 0;

    /**
     * @brief Reads the actual measured pressure from the regulator's feedback.
     * @return The measured pressure in Bar.
     */
    virtual float getActualPressure() = 0;
};



#endif //FIRMWARE_PRESSURECONTROL_H
