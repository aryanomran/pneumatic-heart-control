/*
 * Created by Aryan Abbasgholitabaromran
 *
 * This file implements the logic for the ControlTask C++ class.
 */


#include "Tasks/control_task.h"

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
 * Its one taks is to "launch" the C++ "run" method.
 */
void ControlTask::taskLauncher(void* argument)
{
    // 1. Cast the void* argument back into a pointer to our class
    ControlTask* taskInstance = static_cast<ControlTask*>(argument);

    // 2. Call the real C++ "run" method
    taskInstance->run();
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

                    // 1. Set the pressure regulator to 1.2 Bar (this is for now then I change it i a way that we can connect it to the GUI)

                    m_pPressureRegulator->setPressure(1.2f);

                    // 2. Close the vacuum valve first (safety)
                    m_pDiastoleValve->deactivate();

                    // 3. Open the pressure valve
                    m_pSystoleValve->activate();

                    break;
                }

                case CMD_DIASTOLE_START:
                {
                    // --- DIASTOLE LOGIC ---

                    // 1. Set the pressure regulator to 0 Bar
                    m_pPressureRegulator->setPressure(0.0f);

                    // 2. Close the pressure valve first (safety)
                    m_pSystoleValve->deactivate();

                    // 3. Open the vacuum valve
                    m_pDiastoleValve->activate();

                    break;
                }
            }
        }
    }
}