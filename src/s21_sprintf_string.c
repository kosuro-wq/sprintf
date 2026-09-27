#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#include "s21_string.h"

void s21_buffer_append(
    char *str,
    size_t *index,
    char c
) {
    str[(*index)++] = c;
    str[*index] = '\0';
}

void s21_append_str(
    char *str,
    size_t *index,
    const char *src
) {
    while (*src) {
        str[(*index)++] = *src++;
    }
    str[*index] = '\0';
}

void s21_append_padding(
    char *str,
    size_t *index,
    int count,
    char c
) {
    for (int i = 0; i < count; i++) {
        str[(*index)++] = c;
    }
    str[*index] = '\0';
}

void s21_format_char(
    char *str,
    size_t *index,
    s21_spec_t *spec,
    va_list *args
) {
    char c = (char)va_arg(*args, int);

    int padding = 0;

    if (spec->width > 1) {
        padding = spec->width - 1;
    }

    if (!spec->flag_minus) {
        s21_append_padding(
            str,
            index,
            padding,
            ' '
        );
    }

    s21_buffer_append(
        str,
        index,
        c
    );

    if (spec->flag_minus) {
        s21_append_padding(
            str,
            index,
            padding,
            ' '
        );
    }
}


void s21_format_string(
    char *str,
    size_t *index,
    s21_spec_t *spec,
    va_list *args
) {
    const char *s = va_arg(*args, const char *);

    if (s == NULL) {
        s = "(null)";
    }

    int len = 0;

    while (s[len] != '\0') {
        len++;
    }

    if (
        spec->precision_is_set &&
        spec->precision < len
    ) {
        len = spec->precision;
    }

    int padding = 0;

    if (spec->width > len) {
        padding = spec->width - len;
    }

    if (!spec->flag_minus) {
        s21_append_padding(
            str,
            index,
            padding,
            ' '
        );
    }

    for (int i = 0; i < len; i++) {
        str[(*index)++] = s[i];
    }
    str[*index] = '\0';


    if (spec->flag_minus) {
        s21_append_padding(
            str,
            index,
            padding,
            ' '
        );
    }
}