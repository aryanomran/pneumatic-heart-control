//
// Created by Aryan Abbasgholitabaromran on 20/10/2025.
//

#ifndef FIRMWARE_ISOLENOIDVALVE_H
#define FIRMWARE_ISOLENOIDVALVE_H

/**
 * @file ISolenoidValve.h
 * @brief Abstract interface for a generic solenoid valve.
 *
 * RTOS tasks will use this to open and close the pneumatic valves.
 */

#pragma once

class ISolenoidValve {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~ISolenoidValve() = default;

    /**
     * @brief Activates the valve (opens a Normally Closed valve).
     */
    virtual void activate() = 0;

    /**
     * @brief Deactivates the valve (closes a Normally Closed valve).
     */
    virtual void deactivate() = 0;
};

#endif //FIRMWARE_ISOLENOIDVALVE_H
