#include <check.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include "s21_string.h"


START_TEST(test_text) {
    char expected[256];
    char actual[256];

    sprintf(expected, "Hello %s!", "world");
    s21_sprintf(actual, "Hello %s!", "world");

    ck_assert_str_eq(actual, expected);
}
END_TEST

START_TEST(test_percent) {
    char expected[256];
    char actual[256];

    sprintf(expected, "100%%");
    s21_sprintf(actual, "100%%");

    ck_assert_str_eq(actual, expected);
}
END_TEST


START_TEST(test_char) {
    char expected[256];
    char actual[256];

    sprintf(expected, "%c", 'A');
    s21_sprintf(actual, "%c", 'A');

    ck_assert_str_eq(actual, expected);
}
END_TEST

START_TEST(test_string) {
    char expected[256];
    char actual[256];

    sprintf(expected, "%10.5s", "Hello World");
    s21_sprintf(actual, "%10.5s", "Hello World");

    ck_assert_str_eq(actual, expected);
}
END_TEST

START_TEST(test_null_string) {
    char expected[256];
    char actual[256];
    char *null_str = NULL;

    sprintf(expected, "%s", null_str);
    s21_sprintf(actual, "%s", null_str);

    ck_assert_str_eq(actual, expected);
}
END_TEST


START_TEST(test_int) {
    char expected[256];
    char actual[256];

    sprintf(expected, "%d %i", -123, 456);
    s21_sprintf(actual, "%d %i", -123, 456);

    ck_assert_str_eq(actual, expected);
}
END_TEST

START_TEST(test_int_flags) {
    char expected[256];
    char actual[256];

    sprintf(expected, "%+08d % d", 123, 123);
    s21_sprintf(actual, "%+08d % d", 123, 123);

    ck_assert_str_eq(actual, expected);
}
END_TEST

START_TEST(test_int_width_precision) {
    char expected[256];
    char actual[256];

    sprintf(expected, "%10.5d", 123);
    s21_sprintf(actual, "%10.5d", 123);

    ck_assert_str_eq(actual, expected);
}
END_TEST

START_TEST(test_unsigned) {
    char expected[256];
    char actual[256];

    sprintf(expected, "%u %o %x %X", 123456u, 123456u, 123456u, 123456u);
    s21_sprintf(actual, "%u %o %x %X",
                123456u, 123456u, 123456u, 123456u);

    ck_assert_str_eq(actual, expected);
}
END_TEST

START_TEST(test_hash) {
    char expected[256];
    char actual[256];

    sprintf(expected, "%#o %#x %#X", 123, 123, 123);
    s21_sprintf(actual, "%#o %#x %#X", 123, 123, 123);

    ck_assert_str_eq(actual, expected);
}
END_TEST


START_TEST(test_dynamic_width) {
    char expected[256];
    char actual[256];

    sprintf(expected, "%*d", 10, 123);
    s21_sprintf(actual, "%*d", 10, 123);

    ck_assert_str_eq(actual, expected);
}
END_TEST

START_TEST(test_negative_width) {
    char expected[256];
    char actual[256];

    sprintf(expected, "%*d", -10, 123);
    s21_sprintf(actual, "%*d", -10, 123);

    ck_assert_str_eq(actual, expected);
}
END_TEST

START_TEST(test_dynamic_precision) {
    char expected[256];
    char actual[256];

    sprintf(expected, "%.*f", 3, 12.34567);
    s21_sprintf(actual, "%.*f", 3, 12.34567);

    ck_assert_str_eq(actual, expected);
}
END_TEST

START_TEST(test_float) {
    char expected[256];
    char actual[256];

    sprintf(expected, "%.2f", 123.456);
    s21_sprintf(actual, "%.2f", 123.456);

    ck_assert_str_eq(actual, expected);
}
END_TEST

START_TEST(test_float_formats) {
    char expected[256];
    char actual[256];

    sprintf(expected, "%.3e %.3E %.5g %.5G",
            1234.567, 1234.567, 1234.567, 1234.567);

    s21_sprintf(actual, "%.3e %.3E %.5g %.5G",
                1234.567, 1234.567, 1234.567, 1234.567);

    ck_assert_str_eq(actual, expected);
}
END_TEST


START_TEST(test_pointer) {
    char expected[256];
    char actual[256];
    int value = 42;

    sprintf(expected, "%p", (void *)&value);
    s21_sprintf(actual, "%p", (void *)&value);

    ck_assert_str_eq(actual, expected);
}
END_TEST


START_TEST(test_length) {
    char expected[256];
    char actual[256];

    short h = 123;
    long l = 123456789L;

    sprintf(expected, "%hd %ld", h, l);
    s21_sprintf(actual, "%hd %ld", h, l);

    ck_assert_str_eq(actual, expected);
}
END_TEST


START_TEST(test_combined) {
    char expected[512];
    char actual[512];

    sprintf(expected,
            "Name: %-10s | ID: %+06d | Score: %.2f | Hex: %#x",
            "Alice", 42, 98.765, 255);

    s21_sprintf(actual,
                "Name: %-10s | ID: %+06d | Score: %.2f | Hex: %#x",
                "Alice", 42, 98.765, 255);

    ck_assert_str_eq(actual, expected);
}
END_TEST


Suite *s21_sprintf_suite(void) {
    Suite *suite = suite_create("s21_sprintf");
    TCase *tc = tcase_create("core");

    tcase_add_test(tc, test_text);
    tcase_add_test(tc, test_percent);

    tcase_add_test(tc, test_char);
    tcase_add_test(tc, test_string);
    tcase_add_test(tc, test_null_string);

    tcase_add_test(tc, test_int);
    tcase_add_test(tc, test_int_flags);
    tcase_add_test(tc, test_int_width_precision);
    tcase_add_test(tc, test_unsigned);
    tcase_add_test(tc, test_hash);

    tcase_add_test(tc, test_dynamic_width);
    tcase_add_test(tc, test_negative_width);
    tcase_add_test(tc, test_dynamic_precision);

    tcase_add_test(tc, test_float);
    tcase_add_test(tc, test_float_formats);

    tcase_add_test(tc, test_pointer);
    tcase_add_test(tc, test_length);

    tcase_add_test(tc, test_combined);

    suite_add_tcase(suite, tc);

    return suite;
}

int main(void) {
    Suite *suite = s21_sprintf_suite();
    SRunner *runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);
    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return failed == 0 ? 0 : 1;
}