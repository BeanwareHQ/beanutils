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

b_errno_t b_array_init(BeanArray* array) {
    return b_array_init_with_size(array, _BEAN_ARRAY_GROWTH_FACTOR);
}

b_errno_t b_array_init_with_size(BeanArray* array, size_t cap) {
    if (array->cap != 0)
        return STATUS_INVALID_OPERATION;

    *array = (BeanArray){
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

b_errno_t b_array_deinit(BeanArray* array) {
    if (array->cap == 0)
        return STATUS_INVALID_OPERATION;

    for (size_t i = 0; i < array->len; i++) {
        free(array->data[i]);
    }

    free(array->data);
    array->cap = 0;

    return STATUS_SUCCESS;
}

b_errno_t b_array_reserve(BeanArray* array, size_t size) {
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

b_errno_t b_array_expand(BeanArray* array) {
    size_t newcap = array->cap * _BEAN_ARRAY_GROWTH_FACTOR;
    return b_array_reserve(array, newcap);
}

b_errno_t b_array_shrink(BeanArray* array) {
    size_t newcap = array->cap / _BEAN_ARRAY_GROWTH_FACTOR;

    if (newcap == 0)
        return STATUS_INVALID_OPERATION;

    return b_array_reserve(array, newcap);
}

b_errno_t b_array_pop(BeanArray* array) {
    if (array->len - 1 < array->cap / _BEAN_ARRAY_GROWTH_FACTOR) {
        b_errno_t stat;
        if ((stat = b_array_shrink(array)) != STATUS_SUCCESS)
            return stat;
    }

    void* elem = array->data[--array->len];
    free(elem);
    elem = NULL;

    return STATUS_SUCCESS;
}

b_errno_t b_array_push(BeanArray* array, void* newelem) {
    if (array->len + 1 > array->cap) {
        b_errno_t stat;
        if ((stat = b_array_expand(array)) != STATUS_SUCCESS)
            return stat;
    }

    array->data[array->len++] = newelem;

    return STATUS_SUCCESS;
}

b_errno_t b_array_append(BeanArray* first, BeanArray* second) {
    b_errno_t stat;
    size_t newlen = first->len + second->len;
    size_t currcap;

    if (first->cap >= second->cap)
        currcap = first->cap;
    else
        currcap = second->cap;

    while (newlen < (currcap *= _BEAN_ARRAY_GROWTH_FACTOR))
        ;

    if ((stat = b_array_reserve(first, currcap)) != STATUS_SUCCESS)
        return stat;

    memcpy(&first->data[first->len], second->data, sizeof(void*) * second->len);

    first->len += second->len;
    free(second->data);
    second->cap = 0;

    return STATUS_SUCCESS;
}

bool b_array_is_equal(BeanArray* array, BeanArray* rhs, size_t size) {
    if (array->len != rhs->len)
        return false;

    for (size_t i = 0; i < array->len; i++)
        if (memcmp(array->data[i], rhs->data[i], size) != 0)
            return false;

    return true;
}

b_errno_t b_array_insert(BeanArray* array, void* elem, size_t index) {
    if (array->len + 1 > array->cap) {
        b_errno_t stat;
        if ((stat = b_array_expand(array)) != STATUS_SUCCESS)
            return stat;
    }

    memmove(&array->data[index], &array->data[index + 1],
            sizeof(void*) * array->len - index);
    array->data[index] = elem;

    return STATUS_SUCCESS;
}

b_errno_t b_array_remove(BeanArray* array, size_t index) {
    if (array->len - 1 < array->cap / _BEAN_ARRAY_GROWTH_FACTOR) {
        b_errno_t stat;
        if ((stat = b_array_shrink(array)) != STATUS_SUCCESS)
            return stat;
    }

    free(array->data[index]);
    memmove(&array->data[index + 1], &array->data[index],
            sizeof(void*) * array->len - index);

    return STATUS_SUCCESS;
}

BeanArrayView b_array_get_view(BeanArray* array, size_t start, size_t finish) {
    BeanArrayView view = {0};

    view.len = finish - start;
    view.elems = &array->data[start];

    return view;
}

b_errno_t b_array_slice(BeanArray* array, BeanArray* newarray, size_t start,
                        size_t finish, size_t elemsize) {
    BeanArray res = {0};
    b_array_init(&res);

    if (start < 0)
        start = 0;
    if (finish >= array->len)
        finish = array->len - 1;

    for (size_t i = start; i != finish; i++) {
        b_errno_t pushstat;
        void* elem = NULL;

        memcpy(elem, array->data[i], elemsize);
        if ((pushstat = b_array_push(&res, elem)) != STATUS_SUCCESS)
            return pushstat;
    }

    *newarray = res;

    return STATUS_SUCCESS;
}

b_errno_t b_array_clone(BeanArray* array, BeanArray* newarray,
                        size_t elemsize) {
    BeanArray res = {0};
    b_array_init(&res);

    for (size_t i = 0; i < array->len; i++) {
        b_errno_t pushstat;
        void* elem = NULL;

        memcpy(elem, array->data[i], elemsize);
        if ((pushstat = b_array_push(&res, elem)) != STATUS_SUCCESS)
            return pushstat;
    }

    *newarray = res;

    return STATUS_SUCCESS;
}

bool b_arrview_equal(const BeanArrayView* slice, const BeanArrayView* rhs,
                     size_t size) {
    if (slice->len != rhs->len)
        return false;

    for (size_t i = 0; i < slice->len; i++)
        if (memcmp(slice->elems[i], rhs->elems[i], size) != 0)
            return false;

    return true;
}
