#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct test_suite_t test_suite_t;

typedef struct {
    const char* name;
    void (*func)(test_suite_t* suite);
} test_case_t;

struct test_suite_t {
    const char* suite_name;
    test_case_t* tests;
    size_t test_count;
    size_t passed;
    size_t failed;
};

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "  FAIL: %s\n", message); \
            suite->failed++; \
            return; \
        } \
    } while(0)

#define TEST_ASSERT_EQ(a, b, message) \
    do { \
        if ((a) != (b)) { \
            fprintf(stderr, "  FAIL: %s (expected %ld, got %ld)\n", message, (long)(b), (long)(a)); \
            suite->failed++; \
            return; \
        } \
    } while(0)

#define TEST_ASSERT_NE(a, b, message) \
    do { \
        if ((a) == (b)) { \
            fprintf(stderr, "  FAIL: %s (expected not %ld, got %ld)\n", message, (long)(b), (long)(a)); \
            suite->failed++; \
            return; \
        } \
    } while(0)

#define TEST_ASSERT_PTR_EQ(a, b, message) \
    do { \
        if ((a) != (b)) { \
            fprintf(stderr, "  FAIL: %s (pointers not equal)\n", message); \
            suite->failed++; \
            return; \
        } \
    } while(0)

#define TEST_ASSERT_PTR_NE(a, b, message) \
    do { \
        if ((a) == (b)) { \
            fprintf(stderr, "  FAIL: %s (pointers equal)\n", message); \
            suite->failed++; \
            return; \
        } \
    } while(0)

#define TEST_ASSERT_NULL(ptr, message) \
    do { \
        if ((ptr) != NULL) { \
            fprintf(stderr, "  FAIL: %s (expected NULL)\n", message); \
            suite->failed++; \
            return; \
        } \
    } while(0)

#define TEST_ASSERT_NOT_NULL(ptr, message) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "  FAIL: %s (expected non-NULL)\n", message); \
            suite->failed++; \
            return; \
        } \
    } while(0)

#define TEST_ASSERT_STR_EQ(a, b, message) \
    do { \
        if (strcmp((a), (b)) != 0) { \
            fprintf(stderr, "  FAIL: %s (expected '%s', got '%s')\n", message, (b), (a)); \
            suite->failed++; \
            return; \
        } \
    } while(0)

#define RUN_TEST(suite, test_func) \
    do { \
        printf("  Running %s...\n", #test_func); \
        suite->failed = 0; \
        suite->passed = 0; \
        test_func(suite); \
        if (suite->failed == 0) { \
            suite->passed++; \
            printf("  PASS: %s\n", #test_func); \
        } \
    } while(0)

#define RUN_SUITE(suite_name_str, ...) \
    do { \
        test_case_t tests[] = { __VA_ARGS__ }; \
        test_suite_t suite; \
        suite.suite_name = suite_name_str; \
        suite.tests = tests; \
        suite.test_count = sizeof(tests) / sizeof(tests[0]); \
        suite.passed = 0; \
        suite.failed = 0; \
        printf("\n=== Running test suite: %s ===\n", suite_name_str); \
        size_t total_passed = 0; \
        size_t total_failed = 0; \
        for (size_t i = 0; i < suite.test_count; i++) { \
            printf("\nTest %zu/%zu: %s\n", i + 1, suite.test_count, suite.tests[i].name); \
            suite.failed = 0; \
            suite.passed = 0; \
            suite.tests[i].func(&suite); \
            if (suite.failed == 0) { \
                total_passed++; \
                printf("  PASS: %s\n", suite.tests[i].name); \
            } else { \
                total_failed++; \
            } \
        } \
        suite.passed = total_passed; \
        suite.failed = total_failed; \
        printf("\n=== Test suite %s: %zu passed, %zu failed ===\n\n", \
               suite_name_str, suite.passed, suite.failed); \
        if (suite.failed > 0) { \
            exit(1); \
        } \
    } while(0)

#ifdef __cplusplus
}
#endif

#endif /* TESTS_H */

