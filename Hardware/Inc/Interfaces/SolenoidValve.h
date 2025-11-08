//
// Created by Aryan Abbasgholitabaromran on 20/10/2025.
//

#ifndef FIRMWARE_SOLENOIDVALVE_H
#define FIRMWARE_SOLENOIDVALVE_H

/**
 * @file SolenoidValve.h  //
 * @brief Abstract interface for a generic solenoid valve.
 *
 * RTOS tasks will use this to open and close the pneumatic valves.
 */

#pragma once

class SolenoidValve {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~SolenoidValve() = default;

    /**
     * @brief Activates the valve (opens a Normally Closed valve).
     */
    virtual void activate() = 0;

    /**
     * @brief Deactivates the valve (closes a Normally Closed valve).
     */
    virtual void deactivate() = 0;
};

#endif //FIRMWARE_SOLENOIDVALVE_H