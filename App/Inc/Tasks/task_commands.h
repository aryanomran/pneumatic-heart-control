//
// Created by Aryan Abbasgholitabaromran on 08/11/2025.
//

/*
 * This is a shared file.
 * It defines the "commands" that the Heartbeat task
 * sends to the Pneumatic Control task.
 */

#ifndef FIRMWARE_TASK_COMMANDS_H
#define FIRMWARE_TASK_COMMANDS_H

// This is the message we will send in the queue
typedef enum {
    CMD_SYSTOLE_START,
    CMD_DIASTOLE_START
} PneumaticCommand_t;

#endif //FIRMWARE_TASK_COMMANDS_H