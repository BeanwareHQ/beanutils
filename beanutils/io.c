/* beanutils: Some data structure implementations and utility functions, I
 * guess.
 *
 * Copyright (c) Eason Qin, 2024.
 *
 * NOTE: This source code form is licensed under the MIT license and comes
 * with ABSOLUTELY NO WARRANTY. For more information, please view the
 * `LICENSE` file at the root of the project.
 */

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "io.h"
#include "logger.h"
#include "string.h"

BeanString b_file_read(FILE* file) {
    BeanString res = {0};
    char currch;

    if (b_string_init(&res) != STATUS_SUCCESS) {
        b_log(LOGLEVEL_FATAL, "could not initialize the BeanString");
        perror("error");
        exit(EXIT_FAILURE);
    }

    while ((currch = fgetc(file)) != EOF) {
        b_string_push(&res, currch);
    }

    return res;
}

BeanString b_file_read_line(FILE* file) {
    BeanString res = {0};
    char currch;

    if (b_string_init(&res) != STATUS_SUCCESS) {
        b_log(LOGLEVEL_FATAL, "could not initialize the BeanString");
        perror("error");
        exit(EXIT_FAILURE);
    }

    while ((currch = fgetc(file)) != '\n') {
        b_string_push(&res, currch);
    }

    return res;
}

void b_file_write(FILE* file, BeanString* str) {
    for (size_t i = 0; i < str->len; i++) {
        fputc(str->data[i], file);
    }
}
