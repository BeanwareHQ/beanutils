/* beanutils: Some data structure implementations and utility functions, I
 * guess.
 *
 * Copyright (c) Eason Qin, 2024.
 *
 * NOTE: This source code form is licensed under the MIT license and comes
 * with ABSOLUTELY NO WARRANTY. For more information, please view the
 * `LICENSE` file at the root of the project.
 */

#include <stdio.h>

#include "logger.h"

void b_log(b_loglevel_t lvl, const char* restrict format, ...) {
    va_list args;
    char* logprefix_buf;

    switch (lvl) {
        case LOGLEVEL_LOG: {
            logprefix_buf = "[INFO] ";
        } break;
        case LOGLEVEL_WARN: {
            logprefix_buf = "[WARN] ";
        } break;
        case LOGLEVEL_ERROR: {
            logprefix_buf = "[ERROR] ";
        } break;
        case LOGLEVEL_FATAL: {
            logprefix_buf = "[FATAL] ";
        } break;
        default:
            logprefix_buf = "[INFO] ";
    }

    fprintf(stderr, "%s", logprefix_buf);
    fprintf(stderr, format, args);
    putchar('\n');
}
