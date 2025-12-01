/*
 * Created by Aryan Abbasgholitabaromran
 *
 * Implementation of the SensorTask C++ class.
 *
 *
 *
 *
 */

#include "Tasks/sensor_task.h"
#include <stdlib.h>
#include <math.h>

// Define pi if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Constructor – store the pointers we will use later                  */

SensorTask::SensorTask(PressureSensor* pAortic,
                       PressureSensor* pPulmonary,
                       PressureControl* pRegulator,
                       osMessageQueueId_t queue)
        : m_pAortic(pAortic)
        , m_pPulmonary(pPulmonary)
        , m_pRegulator(pRegulator)
        , m_queue(queue)
{
    // Seed the random number generator
    srand(osKernelGetTickCount());
}

/* start() – create the FreeRTOS thread                                 */

void SensorTask::start()
{
    const osThreadAttr_t attr = {
            .name       = "SensorTask",
            .stack_size = 512 * 4,
            .priority   = osPriorityAboveNormal
    };

    osThreadNew(taskLauncher, this, &attr);
}


/* taskLauncher – static trampoline required by FreeRTOS                */

void SensorTask::taskLauncher(void* argument)
{
    auto* self = static_cast<SensorTask*>(argument);
    self->run();               // never returns
}


/* run() – the real periodic sensor loop                               */

void SensorTask::run()
{
    SensorData_t data{};                     // zero-initialised
    uint32_t nextWake = osKernelGetTickCount();

    // State machine for our pulse wave
    enum PulseState { STATE_DIASTOLE, STATE_SYSTOLE };
    PulseState currentState = STATE_DIASTOLE;
    uint32_t ticksInState = 0; //  10ms-step timer

    // Constants for the simulation
    const float SYSTOLE_TICKS = 300.0f / SAMPLING_PERIOD_MS;  // 30 ticks
    const float DIASTOLE_TICKS = 700.0f / SAMPLING_PERIOD_MS; // 70 ticks

    for (;;) {
        /* ---- 1. Stable 10 ms timing -------------------------------- */
        nextWake += SAMPLING_PERIOD_MS;
        osDelayUntil(nextWake);
        ticksInState++; // Increment our timer every 10ms

        /* ---- 2. Fill the timestamp --------------------------------- */
        data.timestamp_ms = osKernelGetTickCount();

        /* ---- 3. Read the REAL regulator pressure ------------------- */
        // FIX: The new driver API requires passing a float by reference.
        float currentPressure = 0.0f;
        m_pRegulator->getActualPressure_Bar(currentPressure);
        data.regulator_pressure_bar = currentPressure;

        /* ---- 4. Check for State Change ----------------------------- */
        PulseState newState = (data.regulator_pressure_bar > 0.6f) ? STATE_SYSTOLE : STATE_DIASTOLE;

        if (newState != currentState) {
            // The state just changed! Reset the timer.
            currentState = newState;
            ticksInState = 0;
        }

        /* ---- 5. Generate MOCKED Aortic/Pulmonary values ------------ */
        float phase;
        float jitter = (float)(rand() % 100 - 50) / 100.0f; // +/- 0.5 mmHg jitter

        if (currentState == STATE_SYSTOLE)
        {
            // --- SYSTOLE (300ms): Rapid RISE from 80 to 120 ---
            phase = (float)ticksInState / SYSTOLE_TICKS;
            if (phase > 1.0f) phase = 1.0f; // Clamp

            // Use sin(0 -> PI/2) for a 0.0 -> 1.0 rise curve
            float riseCurve = sin(phase * M_PI / 2.0f);

            // Aortic: 80 -> 120
            data.aortic_pressure_mmhg = 80.0f + (40.0f * riseCurve) + jitter;
            // Pulmonary: 10 -> 25
            data.pulmonary_pressure_mmhg = 10.0f + (15.0f * riseCurve) + jitter;
        }
        else // (currentState == STATE_DIASTOLE)
        {
            // --- DIASTOLE (700ms): Slow DECAY from 120 to 80 ---
            phase = (float)ticksInState / DIASTOLE_TICKS;
            if (phase > 1.0f) phase = 1.0f; // Clamp

            // Use cos(0 -> PI/2) for a 1.0 -> 0.0 decay curve
            float decayCurve = cos(phase * M_PI / 2.0f);

            // Aortic: Decays from 120 to 80
            data.aortic_pressure_mmhg = 80.0f + (40.0f * decayCurve) + jitter;
            // Pulmonary: Decays from 25 to 10
            data.pulmonary_pressure_mmhg = 10.0f + (15.0f * decayCurve) + jitter;
        }

        /* ---- 6. Push the packet into the logging queue ------------- */
        osMessageQueuePut(m_queue, &data, 0U, 0U);
    }
}