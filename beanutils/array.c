/* beanutils: Some data structure implementations and utility functions, I
 * guess.
 *
 * Copyright (c) Eason Qin, 2024.
 *
 * NOTE: This source code form is licensed under the MIT license and comes
 * with ABSOLUTELY NO WARRANTY. For more information, please view the
 * `LICENSE` file at the root of the project.
 */

#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "common.h"

Bean_Status_t Bean_Array_init(Bean_Array* array) {
    return Bean_Array_initWithSize(array, _BEAN_ARRAY_GROWTH_FACTOR);
}

Bean_Status_t Bean_Array_initWithSize(Bean_Array* array, size_t cap) {
    if (array->cap != 0)
        return STATUS_INVALID_OPERATION;

    *array = (Bean_Array){
        .len = 0,
        .cap = cap,
        .data = NULL,
    };

    array->data = calloc(cap, sizeof(void*));
    if (array->data == NULL) {
        array->cap = 0;
        return STATUS_FAILED_ALLOC;
    }

    return STATUS_SUCCESS;
}

Bean_Status_t Bean_Array_deinit(Bean_Array* array) {
    if (array->cap == 0)
        return STATUS_INVALID_OPERATION;

    for (size_t i = 0; i < array->len; i++) {
        free(array->data[i]);
    }

    free(array->data);
    array->cap = 0;

    return STATUS_SUCCESS;
}

Bean_Status_t Bean_Array_reserve(Bean_Array* array, size_t size) {
    if (size == 0 && array->cap != 0)
        return STATUS_INVALID_OPERATION;

    array->data = (void**)realloc(array->data, sizeof(void*) * size);

    if (array->data == NULL) {
        array->cap = 0;
        return STATUS_FAILED_ALLOC;
    } else {
        array->cap = size;
        return STATUS_SUCCESS;
    }
}

Bean_Status_t Bean_Array_expand(Bean_Array* array) {
    size_t newcap = array->cap * _BEAN_ARRAY_GROWTH_FACTOR;
    return Bean_Array_reserve(array, newcap);
}

Bean_Status_t Bean_Array_shrink(Bean_Array* array) {
    size_t newcap = array->cap / _BEAN_ARRAY_GROWTH_FACTOR;

    if (newcap == 0)
        return STATUS_INVALID_OPERATION;

    return Bean_Array_reserve(array, newcap);
}

Bean_Status_t Bean_Array_pop(Bean_Array* array) {
    if (array->len - 1 < array->cap / _BEAN_ARRAY_GROWTH_FACTOR) {
        Bean_Status_t stat;
        if ((stat = Bean_Array_shrink(array)) != STATUS_SUCCESS)
            return stat;
    }

    void* elem = array->data[--array->len];
    free(elem);
    elem = NULL;

    return STATUS_SUCCESS;
}

Bean_Status_t Bean_Array_push(Bean_Array* array, void* newelem) {
    if (array->len + 1 > array->cap) {
        Bean_Status_t stat;
        if ((stat = Bean_Array_expand(array)) != STATUS_SUCCESS)
            return stat;
    }

    array->data[array->len++] = newelem;

    return STATUS_SUCCESS;
}

Bean_Status_t Bean_Array_append(Bean_Array* first, Bean_Array* second) {
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

    memcpy(&first->data[first->len], second->data, sizeof(void*) * second->len);

    first->len += second->len;
    free(second->data);
    second->cap = 0;

    return STATUS_SUCCESS;
}

bool Bean_Array_isEqual(Bean_Array* array, Bean_Array* rhs, size_t size) {
    if (array->len != rhs->len)
        return false;

    for (size_t i = 0; i < array->len; i++)
        if (memcmp(array->data[i], rhs->data[i], size) != 0)
            return false;

    return true;
}

Bean_Status_t Bean_Array_insert(Bean_Array* array, void* elem, size_t index) {
    if (array->len + 1 > array->cap) {
        Bean_Status_t stat;
        if ((stat = Bean_Array_expand(array)) != STATUS_SUCCESS)
            return stat;
    }

    memmove(&array->data[index], &array->data[index + 1],
            sizeof(void*) * array->len - index);
    array->data[index] = elem;

    return STATUS_SUCCESS;
}

Bean_Status_t Bean_Array_remove(Bean_Array* array, size_t index) {
    if (array->len - 1 < array->cap / _BEAN_ARRAY_GROWTH_FACTOR) {
        Bean_Status_t stat;
        if ((stat = Bean_Array_shrink(array)) != STATUS_SUCCESS)
            return stat;
    }

    free(array->data[index]);
    memmove(&array->data[index + 1], &array->data[index],
            sizeof(void*) * array->len - index);

    return STATUS_SUCCESS;
}

Bean_ArrayView Bean_Array_getView(Bean_Array* array, size_t start,
                                  size_t finish) {
    Bean_ArrayView view = {0};

    view.len = finish - start;
    view.elems = &array->data[start];

    return view;
}

Bean_Status_t Bean_Array_slice(Bean_Array* array, Bean_Array* newarray,
                               size_t start, size_t finish, size_t elemsize) {
    Bean_Array res = {0};
    Bean_Array_init(&res);

    if (start < 0)
        start = 0;
    if (finish >= array->len)
        finish = array->len - 1;

    for (size_t i = start; i != finish; i++) {
        Bean_Status_t pushstat;
        void* elem = NULL;

        memcpy(elem, array->data[i], elemsize);
        if ((pushstat = Bean_Array_push(&res, elem)) != STATUS_SUCCESS)
            return pushstat;
    }

    *newarray = res;

    return STATUS_SUCCESS;
}

Bean_Status_t Bean_Array_clone(Bean_Array* array, Bean_Array* newarray,
                               size_t elemsize) {
    Bean_Array res = {0};
    Bean_Array_init(&res);

    for (size_t i = 0; i < array->len; i++) {
        Bean_Status_t pushstat;
        void* elem = NULL;

        memcpy(elem, array->data[i], elemsize);
        if ((pushstat = Bean_Array_push(&res, elem)) != STATUS_SUCCESS)
            return pushstat;
    }

    *newarray = res;

    return STATUS_SUCCESS;
}

bool Bean_Slice_isEqual(const Bean_ArrayView* slice, const Bean_ArrayView* rhs,
                        size_t size) {
    if (slice->len != rhs->len)
        return false;

    for (size_t i = 0; i < slice->len; i++)
        if (memcmp(slice->elems[i], rhs->elems[i], size) != 0)
            return false;

    return true;
}
