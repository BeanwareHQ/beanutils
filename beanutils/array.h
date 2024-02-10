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

#define _BEAN_ARRAY_GROWTH_FACTOR 5

typedef struct {
    void** elems;
    size_t len;
    size_t cap;
} Bean_Array;

/**
 * Initializes a new `Bean_Array`
 */
Bean_Status_t Bean_Array_init(Bean_Array* array);

/**
 * Initializes a new `Bean_Array` with the specified capacity.
 */
Bean_Status_t Bean_Array_initWithSize(Bean_Array* array, size_t cap);

/**
 * Reserves a given capacity on a `Bean_Array`.
 */
Bean_Status_t Bean_Array_reserve(Bean_Array* array, size_t size);

/**
 * Deallocates all elements of a `Bean_Array`.
 */
Bean_Status_t Bean_Array_deinit(Bean_Array* array);

/**
 * Expands a `Bean_Array`.
 */
Bean_Status_t Bean_Array_expand(Bean_Array* array);

/**
 * Shrinks a `Bean_Array`.
 */
Bean_Status_t Bean_Array_shrink(Bean_Array* array);

/**
 * Pops an element off a `Bean_Array`.
 */
Bean_Status_t Bean_Array_pop(Bean_Array* array);

/**
 * Adds one element onto a `Bean_Array`.
 */
Bean_Status_t Bean_Array_push(Bean_Array* array, void* newelem);

/**
 * Appends two `Bean_Array`s together, emptying the second array.
 */
Bean_Status_t Bean_Array_append(Bean_Array* first, Bean_Array* second);

/**
 * Check if two `Bean_Array`s are equal.
 */
bool Bean_Array_isEqual(Bean_Array* array, Bean_Array* rhs, size_t size);

/**
 * Insert an element into a `Bean_Array`.
 */
Bean_Status_t Bean_Array_insert(Bean_Array* array, void* elem, size_t index);

/**
 * Removes an element off of a `Bean_Array`.
 */
Bean_Status_t Bean_Array_remove(Bean_Array* array, size_t index);
