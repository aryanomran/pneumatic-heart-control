/*
 * Created by Aryan Abbasgholitabaromran
 *
 * This file defines the C++ class for the Pneumatic Control "Muscle" Task.
 * This class encapsulates all the logic for obeying commands and
 * controlling the hardware.
 */

#ifndef FIRMWARE_CONTROL_TASK_H
#define FIRMWARE_CONTROL_TASK_H

#include "cmsis_os.h"            // For FreeRTOS types
#include "Tasks/task_commands.h" // For the command enum

//          HARDWARE INTERFACES
// These are the "tools" this task needs to do the hardware interactions job.
#include "Interfaces/PressureControl.h"
#include "Interfaces/SolenoidValve.h"

// Boss here means ---> PressureRegulatorDriver
class ControlTask {
public:
    /**
     * @brief Constructs a new ControlTask object.
     * @param pPressureRegulator  Pointer to the "Boss" for the regulator.
     * @param pSystoleValve       Pointer to the "Boss" for the systole valve.
     * @param pDiastoleValve      Pointer to the "Boss" for the diastole valve.
     * @param commandQueue        The queue to LISTEN ON for commands.
     */
    ControlTask(PressureControl* pPressureRegulator,
                SolenoidValve* pSystoleValve,
                SolenoidValve* pDiastoleValve,
                osMessageQueueId_t commandQueue);

    /**
     * @brief Creates and starts the FreeRTOS task.
     * This is the function will be called from main.cpp.
     */
    void start();

private:
    /**
     * @brief The main task loop
     * This function contains the for(;;) loop.
     */
    void run();

    /**
     * @brief The static "trampoline" function for FreeRTOS.
     * @param argument A void pointer to the "this" instance of the class.
     */
    static void taskLauncher(void* argument);

    /**
     * @brief Enters a Safe State (Deflate/Exhale) immediately.
     * Called when a critical hardware error is detected.
     */
    void enterSafeState();

    // --- Private Member Variables (The "Tools") ---
    PressureControl* m_pPressureRegulator;
    SolenoidValve* m_pSystoleValve;
    SolenoidValve* m_pDiastoleValve;
    osMessageQueueId_t m_commandQueue;
};


#endif //FIRMWARE_CONTROL_TASK_H