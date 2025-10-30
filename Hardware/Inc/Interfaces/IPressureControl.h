//
// Created by Aryan Abbasgholitabaromran on 18/10/2025.
//

#ifndef FIRMWARE_IPRESSURECONTROL_H
#define FIRMWARE_IPRESSURECONTROL_H

/**
 * @file IPressureControl.h
 * @brief Abstract interface for a pressure control system.
 *
 * This is the high-level interface for the RTOS tasks (in the App layer)
 * will use to command the pressure regulator.
 *
 * It hides all the implementation details (like the Festo VPPE) from the application.
 */

#pragma once

class IPressureControl {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IPressureControl() = default;

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



#endif //FIRMWARE_IPRESSURECONTROL_H
