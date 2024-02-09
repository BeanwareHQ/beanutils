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
    STATUS_SUCCESS = 0,
    STATUS_GENERIC_FAILURE = -1,
    STATUS_FAILED_ALLOC = -2,
    STATUS_INVALID_OPERATION = -3,
    STATUS_DATA_NOT_INITIALIZED = -4,
} Bean_Status_t;
