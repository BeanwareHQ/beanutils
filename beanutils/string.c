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

#include "./string.h"
#include "common.h"

int32_t Bean_String_init(Bean_String* bs) {
    return Bean_String_initWithCapacity(bs, _BEAN_STRING_INITIAL_CAPACITY);
}

int32_t Bean_String_initWithCapacity(Bean_String* bs, size_t size) {
    *bs = (Bean_String){
        .data = calloc(size, sizeof(char)),
        .cap = size,
        .len = 0,
    };

    if (bs->data == NULL) {
        bs->cap = 0;
        return STATUS_FAILED_ALLOC;
    } else {
        return STATUS_SUCCESS;
    }
}

int32_t Bean_String_initWithCstr(Bean_String* bs, const char* str) {
    Bean_Status_t stat;
    size_t sz = _BEAN_STRING_INITIAL_CAPACITY;

    while ((sz *= _BEAN_STRING_CAPACITY_MULTIPLIER) < strlen(str))
        ;

    if ((stat = Bean_String_initWithCapacity(bs, sz)) != STATUS_SUCCESS)
        return stat;

    for (size_t i = 0; str[i] != '\0';)
        bs->data[bs->len++] = str[i++];

    return STATUS_SUCCESS;
}

int32_t Bean_String_deinit(Bean_String* bs) {
    if (bs->cap == 0)
        return STATUS_INVALID_OPERATION;

    free(bs->data);
    *bs = (Bean_String){0};

    return STATUS_SUCCESS;
}

bool Bean_String_compare(const Bean_String* bs, const Bean_String* rhs) {
    if (bs->len != rhs->len || bs->cap == 0 || rhs->cap || 0)
        return false;

    for (size_t i = 0; i < bs->len; i++)
        if (bs->data[i] != rhs->data[i])
            return false;

    return true;
}

int32_t Bean_String_reserve(Bean_String* bs, size_t size) {
    if (size == 0 && bs->cap != 0)
        return STATUS_INVALID_OPERATION;

    bs->data = (char*)realloc(bs->data, sizeof(char) * size);

    if (bs->data == NULL) {
        bs->cap = 0;
        return STATUS_FAILED_ALLOC;
    } else {
        bs->cap = size;
        return STATUS_SUCCESS;
    }
}

int32_t Bean_String_expand(Bean_String* bs) {
    size_t newcap = bs->cap * _BEAN_STRING_CAPACITY_MULTIPLIER;
    return Bean_String_reserve(bs, newcap);
}

int32_t Bean_String_shrink(Bean_String* bs) {
    size_t newcap = bs->cap / _BEAN_STRING_CAPACITY_MULTIPLIER;

    if (newcap == 0)
        return STATUS_INVALID_OPERATION;

    return Bean_String_reserve(bs, newcap);
}

int32_t Bean_String_concatNum(Bean_String* bs, Bean_String* other,
                              size_t count) {
    size_t sz = bs->cap;
    while ((sz *= _BEAN_STRING_CAPACITY_MULTIPLIER) < bs->len + count)
        ;

    return STATUS_SUCCESS;
}
