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

int32_t Bean_Array_reserve(Bean_Array* array, size_t size) {
    array->elems = (void**)realloc(array->elems, sizeof(void*) * size);

    if (size == 0 && array->cap != 0) {
        return STATUS_INVALID_OPERATION;
    } else if (array->elems == NULL) {
        array->cap = 0;
        return STATUS_FAILED_ALLOC;
    } else {
        array->cap = size;
        return STATUS_SUCCESS;
    }
}

int32_t Bean_Array_expand(Bean_Array* array) {
    size_t newcap = array->cap * _BEAN_ARRAY_GROWTH_FACTOR;
    return Bean_Array_reserve(array, newcap);
}

int32_t Bean_Array_shrink(Bean_Array* array) {
    size_t newcap = array->cap / _BEAN_ARRAY_GROWTH_FACTOR;
    return Bean_Array_reserve(array, newcap);
}

int32_t Bean_Array_pop(Bean_Array* array) {
    if (array->len - 1 < array->cap / _BEAN_ARRAY_GROWTH_FACTOR) {
        Bean_Status_t stat;
        if ((stat = Bean_Array_shrink(array)) != STATUS_SUCCESS)
            return stat;
    }

    void* elem = array->elems[--array->len];
    free(elem);
    elem = NULL;

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

int32_t Bean_Array_append(Bean_Array* first, Bean_Array* second) {
    Bean_Status_t stat;
    size_t newlen = first->len + second->len;
    size_t currcap;

    if (first->cap >= second->cap)
        currcap = first->cap;
    else
        currcap = second->cap;

    while (newlen < (currcap *= _BEAN_ARRAY_GROWTH_FACTOR))
        ;

    if ((stat = Bean_Array_reserve(first, currcap)) != STATUS_SUCCESS)
        return stat;

    memcpy(&first->elems[first->len], second->elems, second->len);

    free(second->elems);
    second->cap = 0;

    return STATUS_SUCCESS;
}

bool Bean_Array_isEqual(Bean_Array* array, Bean_Array* rhs, size_t size) {
    if (array->len != rhs->len)
        return false;

    for (size_t i = 0; i < array->len; i++)
        if (memcmp(array->elems[i], rhs->elems[i], size) != 0)
            return false;

    return true;
}

int32_t Bean_Array_insert(Bean_Array* array, void* elem, size_t index) {
    if (array->len + 1 > array->cap) {
        Bean_Status_t stat;
        if ((stat = Bean_Array_expand(array)) != STATUS_SUCCESS)
            return stat;
    }

    memmove(&array->elems[index], &array->elems[index + 1],
            sizeof(void*) * array->len - index);
    array->elems[index] = elem;

    return STATUS_SUCCESS;
}

int32_t Bean_Array_remove(Bean_Array* array, size_t index) {
    if (array->len - 1 < array->cap / _BEAN_ARRAY_GROWTH_FACTOR) {
        Bean_Status_t stat;
        if ((stat = Bean_Array_shrink(array)) != STATUS_SUCCESS)
            return stat;
    }

    free(array->elems[index]);
    memmove(&array->elems[index + 1], &array->elems[index],
            sizeof(void*) * array->len - index);

    return STATUS_SUCCESS;
}
