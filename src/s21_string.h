#ifndef S21_STRING_H
#define S21_STRING_H

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    int flag_minus;
    int flag_plus;
    int flag_space;
    int flag_hash;
    int flag_zero;

    int width;
    int width_from_args;

    int precision;
    int precision_is_set;
    int precision_from_args;

    char length;
    char specifier;
} s21_spec_t;

bool s21_is_flag(char c);
bool s21_is_specifier(char c);

const char *s21_parse_format(
    const char *format,
    s21_spec_t *spec,
    va_list *args
);

void s21_buffer_append(
    char *str,
    size_t *index,
    char c
);

void s21_append_str(
    char *str,
    size_t *index,
    const char *src
);

void s21_append_padding(
    char *str,
    size_t *index,
    int count,
    char c
);

void s21_format_char(
    char *str,
    size_t *index,
    s21_spec_t *spec,
    va_list *args
);

void s21_format_string(
    char *str,
    size_t *index,
    s21_spec_t *spec,
    va_list *args
);

void s21_format_integer(
    char *str,
    size_t *index,
    s21_spec_t *spec,
    va_list *args
);

void s21_format_float(
    char *str,
    size_t *index,
    s21_spec_t *spec,
    va_list *args
);

void s21_process_specifier(
    char *str,
    size_t *index,
    s21_spec_t *spec,
    va_list *args
);

int s21_sprintf(
    char *str,
    const char *format,
    ...
);

#endif