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
    void** data;
    size_t len;
    size_t cap;
} BeanArray;

typedef struct {
    void** elems;
    size_t len;
} BeanArrayView;

/**
 * Initializes a new `Bean_Array`
 */
b_errno_t b_array_init(BeanArray* array);

/**
 * Initializes a new `Bean_Array` with the specified capacity.
 */
b_errno_t b_array_init_with_size(BeanArray* array, size_t cap);

/**
 * Reserves a given capacity on a `Bean_Array`.
 */
b_errno_t b_array_reserve(BeanArray* array, size_t size);

/**
 * Deallocates all elements of a `Bean_Array`.
 */
b_errno_t b_array_deinit(BeanArray* array);

/**
 * Expands a `Bean_Array`.
 */
b_errno_t b_array_expand(BeanArray* array);

/**
 * Shrinks a `Bean_Array`.
 */
b_errno_t b_array_shrink(BeanArray* array);

/**
 * Pops an element off a `Bean_Array`.
 */
b_errno_t b_array_pop(BeanArray* array);

/**
 * Adds one element onto a `Bean_Array`.
 */
b_errno_t b_array_push(BeanArray* array, void* newelem);

/**
 * Appends two `Bean_Array`s together, emptying the second array.
 */
b_errno_t b_array_append(BeanArray* first, BeanArray* second);

/**
 * Check if two `Bean_Array`s are equal.
 */
bool b_array_equal(BeanArray* array, BeanArray* rhs, size_t size);

/**
 * Insert an element into a `Bean_Array`.
 */
b_errno_t b_array_insert(BeanArray* array, void* elem, size_t index);

/**
 * Removes an element off of a `Bean_Array`.
 */
b_errno_t b_array_remove(BeanArray* array, size_t index);

/**
 * Creates a `Bean_ArrayView` from a `Bean_Array`.
 *
 *  @param start   If this is out of bounds, it will default to 0.
 *  @param finish  If this is out of bounds, it will default to `array->len -
 * 1`.
 */
BeanArrayView b_array_get_view(BeanArray* array, size_t start, size_t finish);

/**
 * Slices a `Bean_Array` into another `Bean_Array`, duplicating all elements.
 *
 *  @param start   If this is out of bounds, it will default to 0.
 *  @param finish  If this is out of bounds, it will default to `array->len -
 * 1`.
 */
b_errno_t b_array_slice(BeanArray* array, BeanArray* newarray, size_t start,
                        size_t finish, size_t elemsize);

/**
 * Clones a `Bean_Array`, including all its contents.
 */
b_errno_t b_array_clone(BeanArray* array, BeanArray* newarray, size_t elemsize);

/**
 * Check if two `Bean_ArrayView`s are equal.
 */
bool b_arrview_equal(const BeanArrayView* slice, const BeanArrayView* rhs,
                     size_t size);
