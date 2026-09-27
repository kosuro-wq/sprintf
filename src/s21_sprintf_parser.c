#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#include "s21_string.h"

bool s21_is_flag(char c) {
    return (
        c == '-' ||
        c == '+' ||
        c == ' ' ||
        c == '#' ||
        c == '0'
    );
}

bool s21_is_specifier(char c) {
    return (
        c == 'c' ||
        c == 'd' ||
        c == 'i' ||
        c == 'f' ||
        c == 'e' ||
        c == 'E' ||
        c == 'g' ||
        c == 'G' ||
        c == 'o' ||
        c == 's' ||
        c == 'u' ||
        c == 'x' ||
        c == 'X' ||
        c == 'p' ||
        c == 'n' ||
        c == '%'
    );
}

const char *s21_parse_format(
    const char *format,
    s21_spec_t *spec,
    va_list *args
) {
    *spec = (s21_spec_t){0};


    while (*format && s21_is_flag(*format)) {
        if (*format == '-') {
            spec->flag_minus = 1;
        } else if (*format == '+') {
            spec->flag_plus = 1;
        } else if (*format == ' ') {
            spec->flag_space = 1;
        } else if (*format == '#') {
            spec->flag_hash = 1;
        } else if (*format == '0') {
            spec->flag_zero = 1;
        }

        format++;
    }


    if (*format == '*') {
        spec->width_from_args = 1;
        spec->width = va_arg(*args, int);

        if (spec->width < 0) {
            spec->flag_minus = 1;
            spec->width = -spec->width;
        }

        format++;
    } else {
        while (*format >= '0' && *format <= '9') {
            spec->width =
                spec->width * 10 +
                (*format - '0');

            format++;
        }
    }


    if (*format == '.') {
        spec->precision_is_set = 1;
        format++;

        if (*format == '*') {
            spec->precision_from_args = 1;
            spec->precision = va_arg(*args, int);

            if (spec->precision < 0) {
                spec->precision_is_set = 0;
                spec->precision = 0;
            }

            format++;
        } else {
            while (*format >= '0' && *format <= '9') {
                spec->precision =
                    spec->precision * 10 +
                    (*format - '0');

                format++;
            }
        }
    }


    if (*format == 'h') {
        spec->length = 'h';
        format++;
        if (*format == 'h') {
            spec->length = 'H';
            format++;
        }
    } else if (*format == 'l') {
        spec->length = 'l';
        format++;
        if (*format == 'l') {
            spec->length = 'L';
            format++;
        }
    } else if (*format == 'L') {
        spec->length = 'L';
        format++;
    }

    /*
     * =========================
     * SPECIFIER
     * =========================
     */

    if (*format && s21_is_specifier(*format)) {
        spec->specifier = *format;
        format++;
    }

    return format;
}