#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_string.h"

void s21_format_float(
    char *str,
    size_t *index,
    s21_spec_t *spec,
    va_list *args
) {
    int is_long_double = (spec->length == 'L');
    long double ld_value = 0;
    double d_value = 0;

    if (is_long_double) {
        ld_value = va_arg(*args, long double);
    } else {
        d_value = va_arg(*args, double);
    }

    char format[128];
    int pos = 0;

    format[pos++] = '%';

    if (spec->flag_minus) format[pos++] = '-';
    if (spec->flag_plus) format[pos++] = '+';
    if (spec->flag_space) format[pos++] = ' ';
    if (spec->flag_hash) format[pos++] = '#';
    if (spec->flag_zero) format[pos++] = '0';

    if (spec->width > 0) {
        pos += snprintf(format + pos, sizeof(format) - pos, "%d", spec->width);
    }

    if (spec->precision_is_set) {
        format[pos++] = '.';
        pos += snprintf(format + pos, sizeof(format) - pos, "%d", spec->precision);
    }

    if (is_long_double) {
        format[pos++] = 'L';
    }

    format[pos++] = spec->specifier;
    format[pos] = '\0';

    int written = 0;
    if (is_long_double) {
        written = snprintf(NULL, 0, format, ld_value);
    } else {
        written = snprintf(NULL, 0, format, d_value);
    }

    if (written < 0) {
        return;
    }

    char *result = (char *)malloc(written + 1);
    if (!result) {
        return;
    }

    if (is_long_double) {
        snprintf(result, written + 1, format, ld_value);
    } else {
        snprintf(result, written + 1, format, d_value);
    }

    s21_append_str(str, index, result);
    free(result);
}