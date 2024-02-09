/* beanutils: Some data structure implementations and utility functions, I
 * guess.
 *
 * Copyright (c) Eason Qin, 2024.
 *
 * NOTE: This source code form is licensed under the MIT license and comes
 * with ABSOLUTELY NO WARRANTY. For more information, please view the
 * `LICENSE` file at the root of the project.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

#include "array.h"

int32_t Bean_Array_init(Bean_Array* array) {
    return Bean_Array_initWithSize(array, _BEAN_ARRAY_GROWTH_FACTOR);
}

int32_t Bean_Array_initWithSize(Bean_Array* array, size_t cap) {
    *array = (Bean_Array){
        .len = 0,
        .cap = cap,
        .elems = NULL,
    };

    array->elems = calloc(cap, sizeof(void*));
    if (array->elems == NULL) {
        array->cap = 0;
        return STATUS_FAILED_ALLOC;
    }

    return STATUS_SUCCESS;
}

int32_t Bean_Array_deinit(Bean_Array* array) {
    if (array->cap == 0)
        return STATUS_INVALID_OPERATION;

    for (size_t i = 0; i < array->len; i++) {
        free(array->elems[i]);
    }

    free(array->elems);
    array->cap = 0;

    return STATUS_SUCCESS;
}

int32_t Bean_Array_expand(Bean_Array* array) {
    size_t newcap = array->cap * _BEAN_ARRAY_GROWTH_FACTOR;
    void** newdata = (void*)realloc(array->elems, sizeof(void*) * newcap);
    if (newdata == NULL)
        return STATUS_FAILED_ALLOC;

    memset(newdata, 0, sizeof(void*) * newcap);
    for (size_t i = 0; i < array->len; i++) {
        newdata[i] = array->elems[i];
    }

    free(array->elems);
    array->elems = newdata;

    return STATUS_SUCCESS;
}

int32_t Bean_Array_push(Bean_Array* array, void* newelem) {
    if (array->len + 1 > array->cap) {
        Bean_Status_t stat;
        if ((stat = Bean_Array_expand(array)) != STATUS_SUCCESS)
            return stat;
    }

    array->elems[array->len++] = newelem;

    return STATUS_SUCCESS;
}
