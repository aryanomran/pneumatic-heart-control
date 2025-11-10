/*
 * Created by Aryan Abbasgholitabaromran
 *
 * Shared data structure for sensor readings.
 * Used by SensorTask → LoggingTask communication.
 */

#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <stdint.h>

/**
 * @brief Snapshot of all sensor values at a given time.
 */
typedef struct {
    uint32_t timestamp_ms;          ///< System tick when sample was taken
    float    aortic_pressure_mmhg;  ///< Aortic pressure in mmHg
    float    pulmonary_pressure_mmhg; ///< Pulmonary pressure in mmHg
    float    regulator_pressure_bar;  ///< Festo VPPE feedback in bar
    // float aortic_flow_lpm;       // Future use
    // float pulmonary_flow_lpm;
} SensorData_t;

#endif // SENSOR_DATA_H