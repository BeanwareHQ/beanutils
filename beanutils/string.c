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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "logger.h"
#include "string.h"

b_errno_t b_string_init(BeanString* bs) {
    return b_string_init_with_capacity(bs, _BEAN_STRING_INITIAL_CAPACITY);
}

b_errno_t b_string_init_with_capacity(BeanString* bs, size_t size) {
    *bs = (BeanString){
        .data = calloc(size + 1, sizeof(char)),
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

b_errno_t b_string_init_with_cstr(BeanString* bs, const char* str) {
    b_errno_t stat;
    size_t sz = _BEAN_STRING_INITIAL_CAPACITY;

    while ((sz *= _BEAN_STRING_CAPACITY_MULTIPLIER) < strlen(str))
        ;

    if ((stat = b_string_init_with_capacity(bs, sz)) != STATUS_SUCCESS)
        return stat;

    for (size_t i = 0; str[i] != '\0'; i++)
        bs->data[bs->len++] = str[i];
    bs->data[bs->len] = '\0';

    return STATUS_SUCCESS;
}

b_errno_t b_string_deinit(BeanString* bs) {
    if (bs->cap == 0)
        return STATUS_INVALID_OPERATION;

    free(bs->data);
    *bs = (BeanString){0};

    return STATUS_SUCCESS;
}

b_errno_t b_string_reserve(BeanString* bs, size_t size) {
    if (size == 0 && bs->cap != 0)
        return STATUS_INVALID_OPERATION;
    else if (bs->cap > bs->len)
        return STATUS_OPERATION_UNNECESSARY;

    bs->data = (char*)realloc(bs->data, sizeof(char) * (size + 1));

    if (bs->data == NULL) {
        bs->cap = 0;
        return STATUS_FAILED_ALLOC;
    } else {
        bs->cap = size;
        return STATUS_SUCCESS;
    }
}

b_errno_t b_string_expand(BeanString* bs) {
    size_t newcap = bs->cap * _BEAN_STRING_CAPACITY_MULTIPLIER;
    return b_string_reserve(bs, newcap);
}

b_errno_t b_string_shrink(BeanString* bs) {
    size_t newcap = bs->cap / _BEAN_STRING_CAPACITY_MULTIPLIER;

    if (newcap == 0)
        return STATUS_INVALID_OPERATION;

    return b_string_reserve(bs, newcap);
}

bool b_string_simplecmp(const BeanString* lhs, const BeanString* rhs) {
    if (lhs->len != rhs->len || lhs->cap == 0 || rhs->cap || 0)
        return false;

    for (size_t i = 0; i < lhs->len; i++)
        if (lhs->data[i] != rhs->data[i])
            return false;

    return true;
}

int32_t b_string_strcmp(const BeanString* lhs, const BeanString* rhs) {
    size_t len = lhs->len;

    if (rhs->len > lhs->len)
        len = rhs->len;

    return strncmp(lhs->data, rhs->data, len);
}

b_errno_t b_string_concatnum(BeanString* bs, const BeanString* other,
                             size_t count) {
    b_errno_t stat;
    size_t sz = bs->cap;

    while ((sz *= _BEAN_STRING_CAPACITY_MULTIPLIER) < bs->len + count)
        ;

    if (other->len > count)
        count = other->len;

    if ((stat = b_string_reserve(bs, sz)) != STATUS_SUCCESS)
        return stat;

    memcpy(&bs->data[bs->len], other->data, other->len);
    bs->data[bs->len] = '\0';

    return STATUS_SUCCESS;
}

b_errno_t b_string_concat(BeanString* bs, const BeanString* other) {
    return b_string_concatnum(bs, other, other->len);
}

b_errno_t b_string_push(BeanString* bs, char ch) {
    b_errno_t stat;
    size_t sz = bs->cap;

    while ((sz *= _BEAN_STRING_CAPACITY_MULTIPLIER) < bs->len + 1)
        ;

    if (bs->cap != sz)
        if ((stat = b_string_reserve(bs, sz)) != STATUS_SUCCESS)
            return stat;

    bs->data[bs->len++] = ch;
    bs->data[bs->len] = '\0';

    return STATUS_SUCCESS;
}

b_errno_t b_string_push_cstr(BeanString* bs, const char* cstr) {
    b_errno_t stat;
    size_t cstr_len = strlen(cstr);
    size_t sz = bs->cap;

    while ((sz *= _BEAN_STRING_CAPACITY_MULTIPLIER) < bs->len + cstr_len)
        ;

    if (bs->cap != sz)
        if ((stat = b_string_reserve(bs, sz)) != STATUS_SUCCESS)
            return stat;

    memcpy(&bs->data[bs->len], cstr, sizeof(char) * cstr_len);
    bs->len += cstr_len;

    return STATUS_SUCCESS;
}

b_errno_t b_string_insert(BeanString* bs, char ch, size_t index) {
    b_errno_t stat;

    if (bs->cap < bs->len + 1)
        if ((stat = b_string_expand(bs)) != STATUS_SUCCESS)
            return stat;

    memmove(&bs->data[index], &bs->data[index + 1],
            sizeof(char) * bs->len - index + 1);
    bs->data[index] = ch;

    return STATUS_SUCCESS;
}

b_errno_t b_string_remove(BeanString* bs, size_t index) {
    b_errno_t stat;

    if (bs->cap / _BEAN_STRING_CAPACITY_MULTIPLIER > bs->len - 1)
        if ((stat = b_string_shrink(bs)) != STATUS_SUCCESS)
            return stat;

    memmove(&bs->data[index + 1], &bs->data[index],
            sizeof(char) * bs->len - index + 1);

    return STATUS_SUCCESS;
}

BeanString b_string_clone(BeanString* bs) {
    BeanString res = {0};
    b_errno_t errno = b_string_init(&res);

    if (errno != STATUS_SUCCESS) {
        b_log(LOGLEVEL_FATAL, "failed to initialize a new BeanString:");
        perror("   internal error");
        exit(EXIT_FAILURE);
    }

    b_string_reserve(bs, bs->cap);

    strcpy(res.data, bs->data);
    res.len = bs->len;
    res.cap = bs->cap;

    return res;
}

char* b_string_clone_into_cstr(BeanString* bs) {
    char* res = calloc(strlen(bs->data) + 1, 1);
    strcpy(res, bs->data);
    return res;
}
