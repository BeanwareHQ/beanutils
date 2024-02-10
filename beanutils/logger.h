/* beanutils: Some data structure implementations and utility functions, I
 * guess.
 *
 * Copyright (c) Eason Qin, 2024.
 *
 * NOTE: This source code form is licensed under the MIT license and comes
 * with ABSOLUTELY NO WARRANTY. For more information, please view the
 * `LICENSE` file at the root of the project.
 */

#pragma once

typedef enum {
    LOGLEVEL_LOG,
    LOGLEVEL_WARN,
    LOGLEVEL_ERROR,
    LOGLEVEL_FATAL,
} Bean_LogLevel;

/**
 * Logs a value to the standard error.
 */
void Bean_Log(Bean_LogLevel lvl, const char* restrict format, ...);
