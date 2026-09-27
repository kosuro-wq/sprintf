#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#include "s21_string.h"

void s21_process_specifier(
    char *str,
    size_t *index,
    s21_spec_t *spec,
    va_list *args
) {
    switch (spec->specifier) {

        case '%':
            s21_buffer_append(
                str,
                index,
                '%'
            );
            break;

        case 'c':
            s21_format_char(
                str,
                index,
                spec,
                args
            );
            break;

        case 's':
            s21_format_string(
                str,
                index,
                spec,
                args
            );
            break;


        case 'd':
        case 'i':
        case 'u':
        case 'o':
        case 'x':
        case 'X':
        case 'p':
            s21_format_integer(
                str,
                index,
                spec,
                args
            );
            break;


        case 'f':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            s21_format_float(
                str,
                index,
                spec,
                args
            );
            break;


        case 'n': {
            if (spec->length == 'h') {
                short *ptr = va_arg(*args, short *);
                if (ptr) *ptr = (short)*index;
            } else if (spec->length == 'l') {
                long *ptr = va_arg(*args, long *);
                if (ptr) *ptr = (long)*index;
            } else if (spec->length == 'L') {
                long long *ptr = va_arg(*args, long long *);
                if (ptr) *ptr = (long long)*index;
            } else {
                int *ptr = va_arg(*args, int *);
                if (ptr) *ptr = (int)*index;
            }
            break;
        }

        default:
            break;
    }
}

int s21_sprintf(
    char *str,
    const char *format,
    ...
) {
    if (!str || !format) {
        return -1;
    }

    va_list args;
    va_start(args, format);

    size_t index = 0;
    s21_spec_t spec;

    while (*format) {


        if (*format != '%') {
            s21_buffer_append(
                str,
                &index,
                *format
            );
            format++;
            continue;
        }


        format++;


        if (*format == '\0') {
            break;
        }


        format =
            s21_parse_format(
                format,
                &spec,
                &args
            );


        s21_process_specifier(
            str,
            &index,
            &spec,
            &args
        );
    }


    str[index] = '\0';

    va_end(args);

    return (int)index;
}