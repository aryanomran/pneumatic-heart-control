/*
 * Created by Aryan Abbasgholitabaromran on 08/11/2025.
 *
 * This file implements the logic for the HeartbeatTask C++ class.
 */


#include "Tasks/heartbeat_task.h"

/**
 * @brief This is the constructor.
 *
 * It "initializes" the class by saving the "tool" (the queue handle)
 * that it will need to do its job.
 *
 * @param controlQueueHandle The queue to send commands TO.
 */
HeartbeatTask::HeartbeatTask(osMessageQueueId_t controlQueueHandle)
        : m_controlQueueHandle(controlQueueHandle)
{

}

/**
 * @brief Creates and starts the FreeRTOS task.
 * will call from main.cpp.
 */
void HeartbeatTask::start()
{
    // 1. Define the attributes for the RTOS task
    const osThreadAttr_t task_attributes = {
            .name = "HeartbeatTask",
            .stack_size = 128 * 4, // 128 words * 4 bytes/word = 512 bytes
            .priority = (osPriority_t) osPriorityHigh, // This task must be high priority
    };

    // 2. Create the new FreeRTOS thread.
    //    - The 1st argument is the "C-style" function
    //    - The 2nd argument is a pointer to *this* C++ object instance.
    osThreadNew(HeartbeatTask::taskLauncher, this, &task_attributes);
}

/**
 * @brief A static "C-style" function that launches C++ class method.
 *
 * @param argument A void pointer to the "this" instance of the class.
 */
void HeartbeatTask::taskLauncher(void* argument)
{
    // 1. Cast the void* argument back into a pointer to the class
    HeartbeatTask* taskInstance = static_cast<HeartbeatTask*>(argument);

    // 2. Call the real C++ "run" method on that specific object
    taskInstance->run();
}

/**
 * @brief The main task loop (the private "job").
 *
 * This is the real, C++ member function that contains the task's
 * infinite loop.
 */
void HeartbeatTask::run()
{
    PneumaticCommand_t cmd; // The message  will send

    // This is the main loop for this task
    for(;;)
    {
        // --- SYSTOLE ---
        // 1. Send the command to start systole
        cmd = CMD_SYSTOLE_START;
        // it is used in the *member variable* m_controlQueueHandle
        osMessageQueuePut(m_controlQueueHandle, &cmd, 0U, 0U);

        // 2. Sleep for the duration of systole.
        // This is a NON-BLOCKING sleep. The RTOS will let
        // the Sensor task run during this time.
        osDelay(SYSTOLE_DURATION_MS);


        // --- DIASTOLE ---
        // 3. Send the command to start diastole
        cmd = CMD_DIASTOLE_START;
        osMessageQueuePut(m_controlQueueHandle, &cmd, 0U, 0U);

        // 4. Sleep for the duration of diastole.
        // The Sensor task will run during this time.
        osDelay(DIASTOLE_DURATION_MS);
    }
}