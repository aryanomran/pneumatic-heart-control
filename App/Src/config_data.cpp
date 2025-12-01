//
// Created by Aryan Abbasgholitabaromran on 17/11/2025.
//
/*
 * This file defines the global instance of the
 * system configuration struct.
 */

#include "../Inc/config_data.h"

/**
 * @brief The single, global instance of the system configuration.
 *
 * This is the one place in memory where the setpoints are stored.
 * It automatically initializes to the default values (1.2f, 0.05f, 300, 700)
 * defined in the header file.
 */
SystemConfig g_systemConfig;