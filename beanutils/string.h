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

#include "common.h"

#define _BEAN_STRING_INITIAL_CAPACITY    12
#define _BEAN_STRING_CAPACITY_MULTIPLIER 5

/**
 * A non-null-terminated Dynamic String on the Heap.
 */
typedef struct {
    char* data;
    size_t len;
    size_t cap;
} Bean_String;

/**
 * A String Builder that has a mutable heap-allocated buffer
 * that can be converted into a `Bean_String`.
 */
typedef struct {
    char* data;
    size_t len;
    size_t cap;
} Bean_StringBuilder;

/**
 * Initializes a new `Bean_String`.
 */
Bean_Status_t Bean_String_init(Bean_String* bs);

/**
 * Initializes a new `Bean_String` with a specified capacity.
 */
Bean_Status_t Bean_String_initWithCapacity(Bean_String* bs, size_t size);

/**
 * Initializes a new `Bean_String` with a C-string already in it.
 */
Bean_Status_t Bean_String_initWithCstr(Bean_String* bs, const char* str);

/**
 * Deinitializes a new `Bean_String`.
 */
Bean_Status_t Bean_String_deinit(Bean_String* bs);

/**
 * Ensures that the buffer size of a `Bean_String` is no less than a given size.
 */
Bean_Status_t Bean_String_reserve(Bean_String* bs, size_t size);

/**
 * Expands the buffer size of a `Bean_String`.
 */
Bean_Status_t Bean_String_expand(Bean_String* bs);

/**
 * Shrinks the buffer size of a `Bean_String`.
 */
Bean_Status_t Bean_String_shrink(Bean_String* bs);

/**
 * Compares two `Bean_String`s.
 */
bool Bean_String_compare(const Bean_String* bs, const Bean_String* rhs);

/**
 * Copies a given amount of characters from one `Bean_String` to another.
 */
Bean_Status_t Bean_String_concatNum(Bean_String* bs, Bean_String* other,
                                    size_t count);

/**
 * Concatenates two `Bean_String`s, leaving the second `Bean_String` intact.
 */
Bean_Status_t Bean_String_concat(Bean_String* bs, Bean_String* other);

/**
 * Pushes one character onto a `Bean_String`.
 */
Bean_Status_t Bean_String_push(Bean_String* bs, char ch);

/**
 * Pushes a C-style string onto a `Bean_String`.
 */
Bean_Status_t Bean_String_pushCstr(Bean_String* bs, const char* cstr);

/**
 * Inserts a character into a `Bean_String` at a given index.
 */
Bean_Status_t Bean_String_insert(Bean_String* bs, char ch, size_t index);

/**
 * Removes a character from a `Bean_String` at a given position.
 */
Bean_Status_t Bean_String_remove(Bean_String* bs, size_t index);

/**
 * Creates a new C-style string from a `Bean_String` by cloning its contents.
 */
Bean_Status_t Bean_String_intoCstr(Bean_String* bs, char* target);
