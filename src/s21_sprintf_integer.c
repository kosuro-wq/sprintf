#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#include "s21_string.h"

static void s21_unsigned_to_base(
    unsigned long long value,
    unsigned base,
    bool upper,
    char *buf
) {
    const char *digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
    char tmp[65];
    int len = 0;

    if (value == 0) {
        tmp[len++] = '0';
    } else {
        while (value > 0) {
            tmp[len++] = digits[value % base];
            value /= base;
        }
    }

    for (int i = 0; i < len; i++) {
        buf[i] = tmp[len - i - 1];
    }
    buf[len] = '\0';
}

static long long s21_get_signed(
    va_list *args,
    char length
) {
    if (length == 'h') {
        return (short)va_arg(*args, int);
    } else if (length == 'l') {
        return va_arg(*args, long);
    } else if (length == 'L') {
        return va_arg(*args, long long);
    } else {
        return va_arg(*args, int);
    }
}

static unsigned long long s21_get_unsigned(
    va_list *args,
    char length
) {
    if (length == 'h') {
        return (unsigned short)va_arg(*args, unsigned int);
    } else if (length == 'l') {
        return va_arg(*args, unsigned long);
    } else if (length == 'L') {
        return va_arg(*args, unsigned long long);
    } else {
        return va_arg(*args, unsigned int);
    }
}

void s21_format_integer(
    char *str,
    size_t *index,
    s21_spec_t *spec,
    va_list *args
) {
    unsigned base = 10;
    bool upper = false;
    bool is_unsigned = false;
    bool is_pointer = false;

    long long signed_value = 0;
    unsigned long long unsigned_value = 0;

    switch (spec->specifier) {
        case 'd':
        case 'i':
            signed_value = s21_get_signed(args, spec->length);
            break;
        case 'u':
            is_unsigned = true;
            unsigned_value = s21_get_unsigned(args, spec->length);
            break;
        case 'o':
            is_unsigned = true;
            base = 8;
            unsigned_value = s21_get_unsigned(args, spec->length);
            break;
        case 'x':
            is_unsigned = true;
            base = 16;
            unsigned_value = s21_get_unsigned(args, spec->length);
            break;
        case 'X':
            is_unsigned = true;
            base = 16;
            upper = true;
            unsigned_value = s21_get_unsigned(args, spec->length);
            break;
        case 'p':
            is_unsigned = true;
            base = 16;
            is_pointer = true;
            unsigned_value = (unsigned long long)(size_t)va_arg(*args, void *);
            break;
        default:
            return;
    }

    char digits[65];

    if (is_unsigned) {
        s21_unsigned_to_base(unsigned_value, base, upper, digits);
    } else {
        unsigned long long magnitude;
        if (signed_value < 0) {
            magnitude = (unsigned long long)(-(signed_value + 1)) + 1;
        } else {
            magnitude = (unsigned long long)signed_value;
        }
        s21_unsigned_to_base(magnitude, 10, false, digits);
    }

    char sign = '\0';
    if (!is_unsigned && !is_pointer) {
        if (signed_value < 0) {
            sign = '-';
        } else if (spec->flag_plus) {
            sign = '+';
        } else if (spec->flag_space) {
            sign = ' ';
        }
    }

    bool zero_value = (digits[0] == '0' && digits[1] == '\0');

    if (spec->precision_is_set && spec->precision == 0 && zero_value && !is_pointer) {
        if (!(spec->flag_hash && base == 8)) {
            digits[0] = '\0';
        }
    }

    int digits_len = 0;
    while (digits[digits_len]) {
        digits_len++;
    }

    char prefix[3] = "";
    int prefix_len = 0;

    if (is_pointer) {
        prefix[0] = '0';
        prefix[1] = 'x';
        prefix[2] = '\0';
        prefix_len = 2;
    } else if (spec->flag_hash && base == 16 && !zero_value) {
        prefix[0] = '0';
        prefix[1] = upper ? 'X' : 'x';
        prefix[2] = '\0';
        prefix_len = 2;
    } else if (spec->flag_hash && base == 8) {
        if ((digits_len > 0 && digits[0] != '0') || (spec->precision_is_set && spec->precision <= digits_len)) {
            prefix[0] = '0';
            prefix[1] = '\0';
            prefix_len = 1;
        }
    }

      
    int precision_zeros = 0;
    if (spec->precision_is_set && spec->precision > digits_len) {
        precision_zeros = spec->precision - digits_len;
    }

      
    int sign_len = (sign != '\0') ? 1 : 0;
    int total_len = sign_len + prefix_len + precision_zeros + digits_len;

    int padding = 0;
    if (spec->width > total_len) {
        padding = spec->width - total_len;
    }

    bool zero_padding = spec->flag_zero && !spec->flag_minus && !spec->precision_is_set;

      
    if (!spec->flag_minus) {
        if (zero_padding) {
            if (sign != '\0') {
                s21_buffer_append(str, index, sign);
                sign = '\0';
            }
            if (prefix_len > 0) {
                s21_append_str(str, index, prefix);
                prefix[0] = '\0';
                prefix_len = 0;
            }
            s21_append_padding(str, index, padding, '0');
        } else {
            s21_append_padding(str, index, padding, ' ');
        }
    }

    if (sign != '\0') {
        s21_buffer_append(str, index, sign);
    }

    if (prefix_len > 0) {
        s21_append_str(str, index, prefix);
    }

    s21_append_padding(str, index, precision_zeros, '0');

    s21_append_str(str, index, digits);

    if (spec->flag_minus) {
        s21_append_padding(str, index, padding, ' ');
    }
}