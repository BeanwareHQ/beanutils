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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "common.h"

#define _BEAN_STRING_INITIAL_CAPACITY    12
#define _BEAN_STRING_CAPACITY_MULTIPLIER 5

/**
 * A null-terminated Dynamic String on the Heap.
 */
typedef struct {
    char* data;
    size_t len;
    size_t cap;
} BeanString;

/**
 * A String Builder that has a mutable heap-allocated buffer
 * that can be converted into a `BeanString`.
 */
typedef struct {
    char* data;
    size_t len;
    size_t cap;
} BeanStringBuilder;

/**
 * Initializes a new `BeanString`.
 */
b_errno_t b_string_init(BeanString* bs);

/**
 * Initializes a new `BeanString` with a specified capacity.
 */
b_errno_t b_string_init_with_capacity(BeanString* bs, size_t size);

/**
 * Initializes a new `BeanString` with a C-string already in it.
 */
b_errno_t b_string_init_with_cstr(BeanString* bs, const char* str);

/**
 * Deinitializes a new `BeanString`.
 */
b_errno_t b_string_deinit(BeanString* bs);

/**
 * Ensures that the buffer size of a `BeanString` is no less than a given size.
 */
b_errno_t b_string_reserve(BeanString* bs, size_t size);

/**
 * Expands the buffer size of a `BeanString`.
 */
b_errno_t b_string_expand(BeanString* bs);

/**
 * Shrinks the buffer size of a `BeanString`.
 */
b_errno_t b_string_shrink(BeanString* bs);

/**
 * Compares two `BeanString`s.
 */
bool b_string_simplecmp(const BeanString* lhs, const BeanString* rhs);

/**
 * Wrapper around `strcmp` but for `BeanString`s.
 */
int32_t b_string_strcmp(const BeanString* lhs, const BeanString* rhs);

/**
 * Copies a given amount of characters from one `BeanString` to another.
 */
b_errno_t b_string_concatnum(BeanString* bs, const BeanString* other,
                             size_t count);

/**
 * Concatenates two `BeanString`s, leaving the second `BeanString` intact.
 */
b_errno_t b_string_concat(BeanString* bs, const BeanString* other);

/**
 * Pushes one character onto a `BeanString`.
 */
b_errno_t b_string_push(BeanString* bs, char ch);

/**
 * Pushes a C-style string onto a `BeanString`.
 */
b_errno_t b_string_push_cstr(BeanString* bs, const char* cstr);

/**
 * Inserts a character into a `BeanString` at a given index.
 */
b_errno_t b_string_insert(BeanString* bs, char ch, size_t index);

/**
 * Removes a character from a `BeanString` at a given position.
 */
b_errno_t b_string_remove(BeanString* bs, size_t index);

/**
 * Clones a `BeanString`.
 */
BeanString b_string_clone(BeanString* bs);

/**
 * Clones the contents of a `BeanString` to a heap-allocated `char*`.
 */
char* b_string_clone_into_cstr(BeanString* bs);
