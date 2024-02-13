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
#include <stdio.h>

#include "common.h"
#include "string.h"

/**
 * Reads a whole file into a `BeanString`.
 */
BeanString b_file_read(FILE* file);

/**
 * Reads a line of a whole file into a `BeanString`.
 */
BeanString b_file_read_line(FILE* file);

/**
 * Writes the contents of a `BeanString` into a file.
 */
void b_file_write(FILE* file, BeanString* str);
