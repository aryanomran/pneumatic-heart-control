/*
 * Created by Aryan Abbasgholitabaromran
 *
 * Declaration of the SensorTask class.
 */

#pragma once

#include "sensor_data.h"                    // From App/Inc
#include "Interfaces/PressureSensor.h"      // ← Fixed path
#include "Interfaces/PressureControl.h"     // ← Fixed path
#include "cmsis_os2.h"

class SensorTask {
public:
    SensorTask(PressureSensor* pAortic,
               PressureSensor* pPulmonary,
               PressureControl* pRegulator,
               osMessageQueueId_t queue);

    void start();

private:
    static void taskLauncher(void* arg);
    void run();

    PressureSensor*   m_pAortic;
    PressureSensor*   m_pPulmonary;
    PressureControl*  m_pRegulator;
    osMessageQueueId_t m_queue;

    static constexpr uint32_t SAMPLING_PERIOD_MS = 10;
};