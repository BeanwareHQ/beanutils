/* beanutils: Some data structure implementations and utility functions, I
 * guess.
 *
 * Copyright (c) Eason Qin, 2024.
 *
 * NOTE: This source code form is licensed under the MIT license and comes
 * with ABSOLUTELY NO WARRANTY. For more information, please view the
 * `LICENSE` file at the root of the project.
 */

#include "string.h"
#include <stdio.h>

void Bean_fputs(const Bean_String* str, FILE* restrict stream);
void Bean_puts(const Bean_String* str);
