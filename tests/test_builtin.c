#include "tests.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Helper function to compare floating point values with tolerance
static int float_equals(double a, double b, double tolerance)
{
    return fabs(a - b) < tolerance;
}

// Helper to extract float from output string
static double parse_float(const char* str)
{
    return strtod(str, NULL);
}

static void test_abs_integer_positive(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(abs(42))\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "abs(42) should be 42");
}

static void test_abs_integer_negative(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(abs(-42))\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "abs(-42) should be 42");
}

static void test_abs_integer_zero(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(abs(0))\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "0", "abs(0) should be 0");
}

static void test_abs_real_positive(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(abs(3.14))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 3.14, 0.001), "abs(3.14) should be 3.14");
}

static void test_abs_real_negative(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(abs(-3.14))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 3.14, 0.001), "abs(-3.14) should be 3.14");
}

static void test_abs_real_zero(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(abs(0.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 0.0, 0.001), "abs(0.0) should be 0.0");
}

static void test_mod(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(mod(10.5, 3.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 1.5, 0.001), "mod(10.5, 3.0) should be 1.5");
}

static void test_pow(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(pow(2.0, 3.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 8.0, 0.001), "pow(2.0, 3.0) should be 8.0");
}

static void test_sqrt(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(sqrt(16.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 4.0, 0.001), "sqrt(16.0) should be 4.0");
}

static void test_exp(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(exp(1.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, M_E, 0.001), "exp(1.0) should be e");
}

static void test_sin(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(sin(0.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 0.0, 0.001), "sin(0.0) should be 0.0");
}

static void test_sin_pi_over_2(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var pi_val = pi()\nprint(sin(pi_val / 2.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 1.0, 0.001), "sin(pi/2) should be 1.0");
}

static void test_cos(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(cos(0.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 1.0, 0.001), "cos(0.0) should be 1.0");
}

static void test_tan(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(tan(0.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 0.0, 0.001), "tan(0.0) should be 0.0");
}

static void test_acos(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(acos(1.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 0.0, 0.001), "acos(1.0) should be 0.0");
}

static void test_atan2(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(atan2(1.0, 1.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    double expected = atan2(1.0, 1.0);
    TEST_ASSERT(float_equals(result, expected, 0.001), "atan2(1.0, 1.0) should match expected value");
}

static void test_log(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(log(1.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 0.0, 0.001), "log(1.0) should be 0.0");
}

static void test_log_e(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var e_val = exp(1.0)\nprint(log(e_val))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 1.0, 0.001), "log(e) should be 1.0");
}

static void test_log10(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(log10(100.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 2.0, 0.001), "log10(100.0) should be 2.0");
}

static void test_log2(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(log2(8.0))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 3.0, 0.001), "log2(8.0) should be 3.0");
}

static void test_ceil(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(ceil(3.14))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 4.0, 0.001), "ceil(3.14) should be 4.0");
}

static void test_ceil_negative(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(ceil(-3.14))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, -3.0, 0.001), "ceil(-3.14) should be -3.0");
}

static void test_floor(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(floor(3.14))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 3.0, 0.001), "floor(3.14) should be 3.0");
}

static void test_floor_negative(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(floor(-3.14))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, -4.0, 0.001), "floor(-3.14) should be -4.0");
}

static void test_round(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(round(3.5))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 4.0, 0.001), "round(3.5) should be 4.0");
}

static void test_round_down(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(round(3.4))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 3.0, 0.001), "round(3.4) should be 3.0");
}

static void test_pi(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(pi())\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, M_PI, 0.001), "pi() should return pi");
}

static void test_slen_empty(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(slen(\"\"))\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "0", "slen(\"\") should be 0");
}

static void test_slen_simple(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(slen(\"hello\"))\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "5", "slen(\"hello\") should be 5");
}

static void test_slen_utf8(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(slen(\"世界\"))\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2", "slen(\"世界\") should be 2 (2 UTF-8 characters)");
}

static void test_abs_with_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x = -10\nprint(abs(x))\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "10", "abs of variable should work");
}

static void test_abs_with_expression(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(abs(5 - 10))\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "5", "abs of expression should work");
}

static void test_pow_fractional(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(pow(4.0, 0.5))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 2.0, 0.001), "pow(4.0, 0.5) should be 2.0");
}

static void test_sqrt_small(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(sqrt(0.25))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 0.5, 0.001), "sqrt(0.25) should be 0.5");
}

static void test_cos_pi(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var pi_val = pi()\nprint(cos(pi_val))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, -1.0, 0.01), "cos(pi) should be -1.0");
}

static void test_nested_builtins(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print(sqrt(pow(3.0, 2.0)))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 3.0, 0.001), "sqrt(pow(3.0, 2.0)) should be 3.0");
}

static void test_abs_real_with_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x = -3.14\nprint(abs(x))\n");
    capture_stdout_end();
    
    double result = parse_float(captured_output);
    TEST_ASSERT(float_equals(result, 3.14, 0.001), "abs of real variable should work");
}

int main(void)
{
    RUN_SUITE("builtin functions",
        {"abs_integer_positive", test_abs_integer_positive},
        {"abs_integer_negative", test_abs_integer_negative},
        {"abs_integer_zero", test_abs_integer_zero},
        {"abs_real_positive", test_abs_real_positive},
        {"abs_real_negative", test_abs_real_negative},
        {"abs_real_zero", test_abs_real_zero},
        {"abs_with_variable", test_abs_with_variable},
        {"abs_with_expression", test_abs_with_expression},
        {"abs_real_with_variable", test_abs_real_with_variable},
        {"mod", test_mod},
        {"pow", test_pow},
        {"pow_fractional", test_pow_fractional},
        {"sqrt", test_sqrt},
        {"sqrt_small", test_sqrt_small},
        {"exp", test_exp},
        {"sin", test_sin},
        {"sin_pi_over_2", test_sin_pi_over_2},
        {"cos", test_cos},
        {"cos_pi", test_cos_pi},
        {"tan", test_tan},
        {"acos", test_acos},
        {"atan2", test_atan2},
        {"log", test_log},
        {"log_e", test_log_e},
        {"log10", test_log10},
        {"log2", test_log2},
        {"ceil", test_ceil},
        {"ceil_negative", test_ceil_negative},
        {"floor", test_floor},
        {"floor_negative", test_floor_negative},
        {"round", test_round},
        {"round_down", test_round_down},
        {"pi", test_pi},
        {"slen_empty", test_slen_empty},
        {"slen_simple", test_slen_simple},
        {"slen_utf8", test_slen_utf8},
        {"nested_builtins", test_nested_builtins}
    );
    
    printf("All builtin function tests passed!\n");
    return 0;
}

