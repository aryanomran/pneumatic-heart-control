/*
 * Created by Aryan Abbasgholitabaromran
 *
 * This file implements the logic for the ControlTask C++ class.
 */

#include "Tasks/control_task.h"
#include "config_data.h"

/**
 * @brief This is the constructor.
 *
 * It "initializes" the class by saving all the
 * hardware interfaces and queue will need.
 */
ControlTask::ControlTask(PressureControl* pPressureRegulator,
                         SolenoidValve* pSystoleValve,
                         SolenoidValve* pDiastoleValve,
                         osMessageQueueId_t commandQueue)
        : m_pPressureRegulator(pPressureRegulator),
          m_pSystoleValve(pSystoleValve),
          m_pDiastoleValve(pDiastoleValve),
          m_commandQueue(commandQueue)
{
}

/**
 * @brief Creates and starts the FreeRTOS task.
 */
void ControlTask::start()
{
    // 1. Define the attributes for our RTOS task
    const osThreadAttr_t task_attributes = {
            .name = "ControlTask",
            .stack_size = 256 * 4,
            .priority = (osPriority_t) osPriorityNormal, // Normal priority
    };

    // 2. Create the new FreeRTOS thread.
    //    Pass a pointer to *this* C++ object instance as the argument.
    osThreadNew(ControlTask::taskLauncher, this, &task_attributes);
}

/**
 * @brief The static "trampoline" function for FreeRTOS.
 *
 * Its one task is to "launch" the C++ "run" method.
 */
void ControlTask::taskLauncher(void* argument)
{
    // 1. Cast the void* argument back into a pointer to our class
    ControlTask* taskInstance = static_cast<ControlTask*>(argument);

    // 2. Call the real C++ "run" method
    taskInstance->run();
}

/**
 * @brief Helper to enter the SAFE STATE (Deflate/Exhale).
 * Called when a hardware error is detected.
 */
void ControlTask::enterSafeState()
{
    // 1. Close the intake valve immediately (Stop filling)
    m_pSystoleValve->deactivate();

    // 2. Open the exhaust/vacuum valve (Release pressure to safe level)
    m_pDiastoleValve->activate();
}

/**
 * @brief The main task loop (the private "job").
 *
 * This is the real, C++ member function that contains the task's
 * infinite loop.
 */
void ControlTask::run()
{
    PneumaticCommand_t cmd; // A variable to hold the received command

    // This is the main loop for this task
    for(;;)
    {
        // 1. Wait forever until a message arrives in the queue.
        osStatus_t status = osMessageQueueGet(m_commandQueue, &cmd, NULL, osWaitForever);

        if (status == osOK) // message = Yes
        {
            // 2. Act on the command
            switch (cmd)
            {
                case CMD_SYSTOLE_START:
                {
                    // --- SYSTOLE LOGIC ---

                    // 1. Try to set the target pressure
                    PeriphStatus regStatus = m_pPressureRegulator->setTargetPressure_Bar(g_systemConfig.systolePressure_bar);

                    // 2. SAFETY CHECK: Did the hardware accept the command?
                    if (regStatus != PeriphStatus::OK) {
                        // CRITICAL ERROR: Regulator failed (e.g. disconnected, unsafe value)
                        // Action: Abort Systole. Go to Safe State.
                        enterSafeState();
                    }
                    else {
                        // Success: Proceed with normal sequence
                        m_pDiastoleValve->deactivate(); // Close vacuum first (Safety)
                        m_pSystoleValve->activate();    // Open pressure
                    }
                    break;
                }

                case CMD_DIASTOLE_START:
                {
                    // --- DIASTOLE LOGIC ---

                    // 1. Try to set the target pressure
                    PeriphStatus regStatus = m_pPressureRegulator->setTargetPressure_Bar(g_systemConfig.diastolePressure_bar);

                    // 2. SAFETY CHECK
                    if (regStatus != PeriphStatus::OK) {
                        // Even if setting diastole pressure fails, we must ensure
                        // we don't leave the high pressure valve open.
                        enterSafeState();
                    }
                    else {
                        // Success: Proceed with normal sequence
                        m_pSystoleValve->deactivate(); // Close pressure first (Safety)
                        m_pDiastoleValve->activate();  // Open vacuum
                    }
                    break;
                }
            }
        }
    }
}