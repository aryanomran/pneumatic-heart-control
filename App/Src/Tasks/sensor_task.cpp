/*
 * Created by Aryan Abbasgholitabaromran
 *
 * Implementation of the SensorTask C++ class.
 */

#include "Tasks/sensor_task.h"


/*  Constructor – store the pointers we will use later                  */

SensorTask::SensorTask(PressureSensor*   pAortic,
                       PressureSensor*   pPulmonary,
                       PressureControl*  pRegulator,
                       osMessageQueueId_t queue)
        : m_pAortic(pAortic)
        , m_pPulmonary(pPulmonary)
        , m_pRegulator(pRegulator)
        , m_queue(queue)
{

}

/*  start() – create the FreeRTOS thread                                 */

void SensorTask::start()
{
    const osThreadAttr_t attr = {
            .name       = "SensorTask",
            .stack_size = 256 * 4,                     // 1 KB stack
            .priority   = osPriorityAboveNormal        // between control & heartbeat
    };

    osThreadNew(taskLauncher, this, &attr);
}


/*  taskLauncher – static trampoline required by FreeRTOS                */

void SensorTask::taskLauncher(void* argument)
{
    auto* self = static_cast<SensorTask*>(argument);
    self->run();               // never returns
}


/*  run() – the real periodic sensor loop                               */

void SensorTask::run()
{
    SensorData_t data{};                     // zero-initialised
    uint32_t nextWake = osKernelGetTickCount();

    for (;;) {
        /* ---- 1. Stable 10 ms timing -------------------------------- */
        nextWake += SAMPLING_PERIOD_MS;
        osDelayUntil(nextWake);

        /* ---- 2. Fill the timestamp --------------------------------- */
        data.timestamp_ms = osKernelGetTickCount();

        /* ---- 3. Read the three sensors ----------------------------- */
        data.aortic_pressure_mmhg    = m_pAortic->readPressure_mmHg();
        data.pulmonary_pressure_mmhg = m_pPulmonary->readPressure_mmHg();
        data.regulator_pressure_bar  = m_pRegulator->getActualPressure();

        /* ---- 4. Push the packet into the logging queue ------------- */
        /*    timeout = 0 → non-blocking, drop if full (acceptable)   */
        osMessageQueuePut(m_queue, &data, 0U, 0U);
    }
}