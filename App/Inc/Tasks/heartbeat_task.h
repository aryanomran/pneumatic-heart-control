/*
 * Created by Aryan Abbasgholitabaromran on 08/11/2025.
 *
 * This file defines the C++ class for the Heartbeat "Pacemaker" Task.
 * This class encapsulates all the logic and data for the task.
 */

#ifndef FIRMWARE_HEARTBEAT_TASK_H
#define FIRMWARE_HEARTBEAT_TASK_H

#include "cmsis_os.h"            // For FreeRTOS types (osMessageQueueId_t)
#include "task_commands.h" // For the PneumaticCommand_t enum

class HeartbeatTask {
public:
    /**
     * @brief Constructs a new HeartbeatTask object.
     * @param controlQueueHandle The queue to send commands TO.
     */
    HeartbeatTask(osMessageQueueId_t controlQueueHandle);

    /**
     * @brief Creates and starts the FreeRTOS task.
     * This is the function that will call from main.cpp.
     */
    void start();

private:
    /**
     * @brief The main task loop (the private "job").
     * This is the function that contains the for(;;) loop.
     */
    void run();

    /**
     * @brief A static "C-style" function that launches the C++ class method.
     * This is a "trampoline" function required by the FreeRTOS API.
     * @param argument A void pointer to the "this" instance of the class.
     */
    static void taskLauncher(void* argument);

    // --- Private Member Variables ---
    osMessageQueueId_t m_controlQueueHandle; // The task's "tool"

    // --- Task Configuration ---
    static const uint32_t SYSTOLE_DURATION_MS = 300;
    static const uint32_t DIASTOLE_DURATION_MS = 700;
};


#endif //FIRMWARE_HEARTBEAT_TASK_H