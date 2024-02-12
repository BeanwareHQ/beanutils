/* beanutils: Some data structure implementations and utility functions, I
 * guess.
 *
 * Copyright (c) Eason Qin, 2024.
 *
 * NOTE: This source code form is licensed under the MIT license and comes
 * with ABSOLUTELY NO WARRANTY. For more information, please view the
 * `LICENSE` file at the root of the project.
 */

#include <stddef.h>
#include <stdio.h>

#include "io.h"

void Bean_fputs(const Bean_String* str, FILE* restrict stream) {
    for (size_t i = 0; i < str->len; i++)
        fprintf(stream, "%c", str->data[i]);
    fputc('\n', stream);
}

void Bean_puts(const Bean_String* str) { return Bean_fputs(str, stdout); }
