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

Bean_Status_t Bean_String_init(Bean_String* bs) {
    return Bean_String_initWithCapacity(bs, _BEAN_STRING_INITIAL_CAPACITY);
}

Bean_Status_t Bean_String_initWithCapacity(Bean_String* bs, size_t size) {
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

Bean_Status_t Bean_String_initWithCstr(Bean_String* bs, const char* str) {
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

Bean_Status_t Bean_String_deinit(Bean_String* bs) {
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

Bean_Status_t Bean_String_reserve(Bean_String* bs, size_t size) {
    if (size == 0 && bs->cap != 0)
        return STATUS_INVALID_OPERATION;
    else if (bs->cap > bs->len)
        return STATUS_OPERATION_UNNECESSARY;

    bs->data = (char*)realloc(bs->data, sizeof(char) * size);

    if (bs->data == NULL) {
        bs->cap = 0;
        return STATUS_FAILED_ALLOC;
    } else {
        bs->cap = size;
        return STATUS_SUCCESS;
    }
}

Bean_Status_t Bean_String_expand(Bean_String* bs) {
    size_t newcap = bs->cap * _BEAN_STRING_CAPACITY_MULTIPLIER;
    return Bean_String_reserve(bs, newcap);
}

Bean_Status_t Bean_String_shrink(Bean_String* bs) {
    size_t newcap = bs->cap / _BEAN_STRING_CAPACITY_MULTIPLIER;

    if (newcap == 0)
        return STATUS_INVALID_OPERATION;

    return Bean_String_reserve(bs, newcap);
}

Bean_Status_t Bean_String_concatNum(Bean_String* bs, Bean_String* other,
                                    size_t count) {
    Bean_Status_t stat;
    size_t sz = bs->cap;
    while ((sz *= _BEAN_STRING_CAPACITY_MULTIPLIER) < bs->len + count)
        ;

    if (other->len > count)
        count = other->len;

    if ((stat = Bean_String_reserve(bs, sz)) != STATUS_SUCCESS)
        return stat;

    for (size_t i = 0; i < count; i++)
        bs->data[bs->len++] = other->data[i];

    return STATUS_SUCCESS;
}

Bean_Status_t Bean_String_concat(Bean_String* bs, Bean_String* other) {
    return Bean_String_concatNum(bs, other, other->len);
}

Bean_Status_t Bean_String_push(Bean_String* bs, char ch) {
    Bean_Status_t stat;
    size_t sz = bs->cap;

    while ((sz *= _BEAN_STRING_CAPACITY_MULTIPLIER) < bs->len + 1)
        ;

    if (bs->cap != sz)
        if ((stat = Bean_String_reserve(bs, sz)) != STATUS_SUCCESS)
            return stat;

    bs->data[bs->len++] = ch;

    return STATUS_SUCCESS;
}

Bean_Status_t Bean_String_pushCstr(Bean_String* bs, const char* cstr) {
    Bean_Status_t stat;
    size_t cstr_len = strlen(cstr);
    size_t sz = bs->cap;

    while ((sz *= _BEAN_STRING_CAPACITY_MULTIPLIER) < bs->len + cstr_len)
        ;

    if (bs->cap != sz)
        if ((stat = Bean_String_reserve(bs, sz)) != STATUS_SUCCESS)
            return stat;

    memcpy(&bs->data[bs->len], cstr, sizeof(char) * cstr_len);
    bs->len += cstr_len;

    return STATUS_SUCCESS;
}

Bean_Status_t Bean_String_insert(Bean_String* bs, char ch, size_t index) {
    Bean_Status_t stat;

    if (bs->cap < bs->len + 1)
        if ((stat = Bean_String_expand(bs)) != STATUS_SUCCESS)
            return stat;

    memmove(&bs->data[index], &bs->data[index + 1],
            sizeof(char) * bs->len - index);
    bs->data[index] = ch;

    return STATUS_SUCCESS;
}

Bean_Status_t Bean_String_remove(Bean_String* bs, size_t index) {
    Bean_Status_t stat;

    if (bs->cap / _BEAN_STRING_CAPACITY_MULTIPLIER > bs->len - 1)
        if ((stat = Bean_String_shrink(bs)) != STATUS_SUCCESS)
            return stat;

    memmove(&bs->data[index + 1], &bs->data[index],
            sizeof(char) * bs->len - index);

    return STATUS_SUCCESS;
}

Bean_Status_t Bean_String_intoCstr(Bean_String* bs, char* target) {
    target = malloc(sizeof(char) * bs->len + 1);

    if (target == NULL)
        return STATUS_FAILED_ALLOC;

    memcpy(target, bs->data, sizeof(char) * bs->len);
    target[bs->len] = '\0';

    return STATUS_SUCCESS;
}
