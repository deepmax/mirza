#include "tests.h"

// ============================================================================
// Basic signed integer tests
// ============================================================================

static void test_i8_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i8 = 42\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print(42");
}

static void test_i8_arithmetic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i8 = 10\nvar b : i8 = 20\nprint(a + b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "30", "should print(30");
}

static void test_i8_negative(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i8 = -5\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-5", "should print(-5");
}

static void test_i16_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i16 = 1000\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1000", "should print(1000");
}

static void test_i16_arithmetic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i16 = 100\nvar b : i16 = 200\nprint(a * b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "20000", "should print(20000");
}

static void test_i32_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i32 = 100000\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "100000", "should print(100000");
}

static void test_i64_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i64 = 2147483647\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2147483647", "should print(2147483647 (max i32)");
}

static void test_i64_large_arithmetic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i64 = 1000000000\nvar b : i64 = 1000000000\nprint(a + b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2000000000", "should print(2000000000");
}

// ============================================================================
// Corner cases: Boundary values
// ============================================================================

static void test_i8_max(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i8 = 127\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "127", "should print(127 (max i8)");
}

static void test_i8_min(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i8 = -128\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-128", "should print(-128 (min i8)");
}

static void test_i16_max(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i16 = 32767\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "32767", "should print(32767 (max i16)");
}

static void test_i16_min(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i16 = -32768\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-32768", "should print(-32768 (min i16)");
}

static void test_i32_max(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i32 = 2147483647\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2147483647", "should print(2147483647 (max i32)");
}

static void test_i32_min(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i32 = -2147483648\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-2147483648", "should print(-2147483648 (min i32)");
}

static void test_i64_max(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i64 = 9223372036854775807\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "9223372036854775807", "should print(9223372036854775807 (max i64)");
}

static void test_i64_min(test_suite_t* suite)
{
    capture_stdout_start();
    // Use -9223372036854775807 - 1 to avoid parsing issues with the literal
    compile_and_run("var x : i64 = -9223372036854775807\nvar y : i64 = x - 1\nprint(y)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-9223372036854775808", "should print(-9223372036854775808 (min i64)");
}

// ============================================================================
// Corner cases: Type conversion and truncation (I8CAST, I16CAST, I32CAST)
// ============================================================================

static void test_i2b_truncation_positive(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i64 = 300\nvar y : i8 = x\nprint(y)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "44", "should print(44 (300 truncated to i8: 300 % 256 = 44)");
}

static void test_i2b_truncation_negative(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i64 = -200\nvar y : i8 = x\nprint(y)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "56", "should print(56 (-200 truncated to i8 wraps to 56)");
}

static void test_i2b_boundary_wrap(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i64 = 128\nvar y : i8 = x\nprint(y)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-128", "should print(-128 (128 wraps to -128 in i8)");
}

static void test_i2s_truncation_positive(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i64 = 100000\nvar y : i16 = x\nprint(y)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-31072", "should print(-31072 (100000 truncated to i16 wraps)");
}

static void test_i2s_truncation_negative(test_suite_t* suite)
{
    capture_stdout_start();
    // -50000 is outside i16 range (-32768 to 32767), so it wraps
    // -50000 mod 65536 = 15536, which when interpreted as signed i16 = 15536 - 65536 = -50000
    // But since we're storing as i16, it wraps to 15536
    compile_and_run("var x : i64 = -50000\nvar y : i16 = x\nprint(y)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "15536", "should print(15536 (-50000 wraps in i16)");
}

static void test_i2s_boundary_wrap(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i64 = 32768\nvar y : i16 = x\nprint(y)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-32768", "should print(-32768 (32768 wraps to -32768 in i16)");
}

// ============================================================================
// Corner cases: Arithmetic edge cases
// ============================================================================

static void test_i8_overflow_arithmetic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i8 = 100\nvar b : i8 = 50\nvar c : i8 = a + b\nprint(c)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-106", "should print(-106 (150 wraps to -106 in i8)");
}

static void test_i8_underflow_arithmetic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i8 = -100\nvar b : i8 = -50\nvar c : i8 = a + b\nprint(c)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "106", "should print(106 (-150 wraps to 106 in i8)");
}

static void test_i16_overflow_multiplication(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i16 = 200\nvar b : i16 = 200\nvar c : i16 = a * b\nprint(c)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-25536", "should print(-25536 (40000 wraps in i16)");
}

static void test_division_by_one(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 100\nvar b : i32 = 1\nprint(a / b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "100", "should print(100 (division by 1)");
}

static void test_modulo_edge_cases(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 100\nvar b : i32 = 100\nprint(a % b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "0", "should print(0 (100 % 100 = 0)");
}

static void test_modulo_negative(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = -10\nvar b : i32 = 3\nprint(a % b)\n");
    capture_stdout_end();
    
    // C-style modulo: -10 % 3 = -1
    TEST_ASSERT_STR_EQ(captured_output, "-1", "should print(-1 (C-style modulo: -10 % 3 = -1)");
}

static void test_modulo_negative_divisor(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 10\nvar b : i32 = -3\nprint(a % b)\n");
    capture_stdout_end();
    
    // C-style modulo: 10 % -3 = 1
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print(1 (C-style modulo: 10 % -3 = 1)");
}

// ============================================================================
// Corner cases: Unary operations on boundaries
// ============================================================================

static void test_unary_minus_i8_min(test_suite_t* suite)
{
    capture_stdout_start();
    // -(-128) = 128, which overflows i8 and wraps to -128 when stored
    // But the unary minus operates on int64, so we get 128, then need to convert back
    compile_and_run("var x : i8 = -128\nvar y : i8 = -x\nprint(y)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-128", "should print(-128 (128 wraps to -128 in i8)");
}

static void test_unary_minus_i16_min(test_suite_t* suite)
{
    capture_stdout_start();
    // -(-32768) = 32768, which overflows i16 and wraps to -32768 when stored
    compile_and_run("var x : i16 = -32768\nvar y : i16 = -x\nprint(y)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-32768", "should print(-32768 (32768 wraps to -32768 in i16)");
}

static void test_unary_not_zero(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i32 = 0\nprint(not x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-1", "should print(-1 (not 0 = -1)");
}

static void test_unary_not_negative(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i32 = -1\nprint(not x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "0", "should print(0 (not -1 = 0)");
}

// ============================================================================
// Corner cases: Bitwise operations with negative numbers
// ============================================================================

static void test_bitwise_and_negative(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = -1\nvar b : i32 = 5\nprint(a & b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "5", "should print(5 (-1 & 5 = 5)");
}

static void test_bitwise_or_negative(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = -1\nvar b : i32 = 0\nprint(a | b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-1", "should print(-1 (-1 | 0 = -1)");
}

static void test_bitwise_xor_negative(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = -1\nvar b : i32 = 15\nprint(a ^ b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-16", "should print(-16 (-1 ^ 15 = -16)");
}

static void test_bitwise_not(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 0\nprint(not a)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-1", "should print(-1 (not 0 = -1)");
}

static void test_bitwise_not_max(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 2147483647\nprint(not a)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-2147483648", "should print(-2147483648 (not max_i32 = min_i32)");
}

// ============================================================================
// Corner cases: Additional edge cases
// ============================================================================

static void test_zero_operations(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 0\nvar b : i32 = 100\nprint(a + b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "100", "should print(100 (0 + 100 = 100)");
}

static void test_identity_operations(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 50\nprint(a * 1)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "50", "should print(50 (50 * 1 = 50)");
}

// ============================================================================
// Corner cases: Mixed type operations
// ============================================================================

static void test_mixed_i8_i16(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i8 = 10\nvar b : i16 = 20\nprint(a + b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "30", "should print(30 (mixed i8 + i16)");
}

static void test_mixed_i32_i64(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 1000\nvar b : i64 = 2000\nprint(a + b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "3000", "should print(3000 (mixed i32 + i64)");
}

static void test_mixed_all_types(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i8 = 1\nvar b : i16 = 2\nvar c : i32 = 3\nvar d : i64 = 4\nprint(a + b + c + d)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "10", "should print(10 (all types mixed)");
}

// ============================================================================
// Corner cases: Complex nested expressions
// ============================================================================

static void test_nested_arithmetic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 10\nvar b : i32 = 20\nvar c : i32 = 5\nprint((a + b) * c - 10)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "140", "should print(140 ((10+20)*5-10 = 140)");
}

static void test_nested_with_modulo(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 100\nvar b : i32 = 7\nprint((a / b) * b + (a % b))\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "100", "should print(100 (division identity)");
}

static void test_nested_bitwise(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 5\nvar b : i32 = 3\nvar c : i32 = 7\nprint((a & b) | (a & c))\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "5", "should print(5 ((5&3)|(5&7) = 1|5 = 5)");
}

// ============================================================================
// Corner cases: Comparisons with edge values
// ============================================================================

static void test_i8_comparison(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i8 = 10\nvar b : i8 = 20\nprint(a < b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print(1 (10 < 20)");
}

static void test_comparison_equal(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 100\nvar b : i32 = 100\nprint(a == b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print(1 (100 == 100)");
}

static void test_comparison_not_equal(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 100\nvar b : i32 = 200\nprint(a != b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print(1 (100 != 200)");
}

static void test_comparison_negative(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = -10\nvar b : i32 = -5\nprint(a < b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print(1 (-10 < -5)");
}

static void test_comparison_boundary(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i8 = 127\nvar b : i8 = -128\nprint(a > b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print(1 (127 > -128)");
}

// ============================================================================
// Corner cases: Control flow with edge values
// ============================================================================

static void test_i8_if_condition(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i8 = 10\nif x > 5 {\nprint(1)\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print(1");
}

static void test_if_zero_condition(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i32 = 0\nif x {\nprint(1)\n} else {\nprint(0)\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "0", "should print(0 (zero is false)");
}

static void test_if_negative_condition(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i32 = -1\nif x {\nprint(1)\n} else {\nprint(0)\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print(1 (negative is true)");
}

static void test_i8_for_loop(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i : i8 = 0; i < 3; i = i + 1 {\nprint(i)\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "012", "should print(012");
}

static void test_i16_for_loop(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i : i16 = 0; i < 5; i = i + 1 {\nprint(i)\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "01234", "should print(01234");
}

// ============================================================================
// Corner cases: Functions with different types
// ============================================================================

static void test_function_i8_param(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func add(a : i8, b : i8) : void {\nret a + b\n}\nprint(add(10, 20))\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "30", "should print(30");
}

static void test_function_i16_param(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func multiply(a : i16, b : i16) : i16 {\nret a * b\n}\nprint(multiply(10, 5))\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "50", "should print(50");
}

static void test_function_i64_return(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func large() : i64 {\nret 2000000000\n}\nprint(large())\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2000000000", "should print(2000000000");
}

static void test_function_type_conversion(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func truncate(x : i64) : i8 {\nret x\n}\nprint(truncate(300))\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "44", "should print(44 (300 truncated to i8)");
}

// ============================================================================
// Corner cases: Assignment and type conversion
// ============================================================================

static void test_i8_assignment(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i8 = 0\nx = 42\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print(42");
}

static void test_i16_assignment(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i16 = 0\nx = 1000\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1000", "should print(1000");
}

static void test_assignment_with_truncation(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i8 = 0\nvar y : i64 = 500\nx = y\nprint(x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-12", "should print(-12 (500 truncated to i8)");
}

// ============================================================================
// Corner cases: Unary operations
// ============================================================================

static void test_i16_unary_minus(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i16 = 100\nprint(-x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-100", "should print(-100");
}

static void test_i32_unary_minus(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i32 = 500\nprint(-x)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-500", "should print(-500");
}

// ============================================================================
// Corner cases: Bitwise operations
// ============================================================================

static void test_i8_bitwise_and(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i8 = 5\nvar b : i8 = 3\nprint(a & b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print(1 (5 & 3 = 1)");
}

static void test_i32_bitwise_xor(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 12\nvar b : i32 = 10\nprint(a ^ b)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "6", "should print(6 (12 ^ 10 = 6)");
}

// ============================================================================
// Corner cases: Complex expressions
// ============================================================================

static void test_complex_i16_expression(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i16 = 10\nvar b : i16 = 20\nvar c : i16 = 5\nprint((a + b) * c)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "150", "should print(150");
}

static void test_complex_i32_expression(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i32 = 100\nvar y : i32 = 50\nprint(x / y + x % y)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2", "should print(2 (100/50 + 100%50 = 2 + 0)");
}

int main(void)
{
    RUN_SUITE("compiler level 2 - signed integer types",
        // Basic signed integer tests
        {"i8_variable", test_i8_variable},
        {"i8_arithmetic", test_i8_arithmetic},
        {"i8_negative", test_i8_negative},
        {"i16_variable", test_i16_variable},
        {"i16_arithmetic", test_i16_arithmetic},
        {"i32_variable", test_i32_variable},
        {"i64_variable", test_i64_variable},
        {"i64_large_arithmetic", test_i64_large_arithmetic},
        
        // Boundary values
        {"i8_max", test_i8_max},
        {"i8_min", test_i8_min},
        {"i16_max", test_i16_max},
        {"i16_min", test_i16_min},
        {"i32_max", test_i32_max},
        {"i32_min", test_i32_min},
        {"i64_max", test_i64_max},
        {"i64_min", test_i64_min},
        
        // Type conversion and truncation (I8CAST, I16CAST, I32CAST)
        {"i2b_truncation_positive", test_i2b_truncation_positive},
        {"i2b_truncation_negative", test_i2b_truncation_negative},
        {"i2b_boundary_wrap", test_i2b_boundary_wrap},
        {"i2s_truncation_positive", test_i2s_truncation_positive},
        {"i2s_truncation_negative", test_i2s_truncation_negative},
        {"i2s_boundary_wrap", test_i2s_boundary_wrap},
        
        // Arithmetic edge cases
        {"i8_overflow_arithmetic", test_i8_overflow_arithmetic},
        {"i8_underflow_arithmetic", test_i8_underflow_arithmetic},
        {"i16_overflow_multiplication", test_i16_overflow_multiplication},
        {"division_by_one", test_division_by_one},
        {"modulo_edge_cases", test_modulo_edge_cases},
        {"modulo_negative", test_modulo_negative},
        {"modulo_negative_divisor", test_modulo_negative_divisor},
        
        // Unary operations on boundaries
        {"unary_minus_i8_min", test_unary_minus_i8_min},
        {"unary_minus_i16_min", test_unary_minus_i16_min},
        {"unary_not_zero", test_unary_not_zero},
        {"unary_not_negative", test_unary_not_negative},
        
        // Bitwise operations with negative numbers
        {"bitwise_and_negative", test_bitwise_and_negative},
        {"bitwise_or_negative", test_bitwise_or_negative},
        {"bitwise_xor_negative", test_bitwise_xor_negative},
        {"bitwise_not", test_bitwise_not},
        {"bitwise_not_max", test_bitwise_not_max},
        
        // Additional edge cases
        {"zero_operations", test_zero_operations},
        {"identity_operations", test_identity_operations},
        
        // Mixed type operations
        {"mixed_i8_i16", test_mixed_i8_i16},
        {"mixed_i32_i64", test_mixed_i32_i64},
        {"mixed_all_types", test_mixed_all_types},
        
        // Complex nested expressions
        {"nested_arithmetic", test_nested_arithmetic},
        {"nested_with_modulo", test_nested_with_modulo},
        {"nested_bitwise", test_nested_bitwise},
        
        // Comparisons with edge values
        {"i8_comparison", test_i8_comparison},
        {"comparison_equal", test_comparison_equal},
        {"comparison_not_equal", test_comparison_not_equal},
        {"comparison_negative", test_comparison_negative},
        {"comparison_boundary", test_comparison_boundary},
        
        // Control flow with edge values
        {"i8_if_condition", test_i8_if_condition},
        {"if_zero_condition", test_if_zero_condition},
        {"if_negative_condition", test_if_negative_condition},
        {"i8_for_loop", test_i8_for_loop},
        {"i16_for_loop", test_i16_for_loop},
        
        // Functions with different types
        {"function_i8_param", test_function_i8_param},
        {"function_i16_param", test_function_i16_param},
        {"function_i64_return", test_function_i64_return},
        {"function_type_conversion", test_function_type_conversion},
        
        // Assignment and type conversion
        {"i8_assignment", test_i8_assignment},
        {"i16_assignment", test_i16_assignment},
        {"assignment_with_truncation", test_assignment_with_truncation},
        
        // Unary operations
        {"i16_unary_minus", test_i16_unary_minus},
        {"i32_unary_minus", test_i32_unary_minus},
        
        // Bitwise operations
        {"i8_bitwise_and", test_i8_bitwise_and},
        {"i32_bitwise_xor", test_i32_bitwise_xor},
        
        // Complex expressions
        {"complex_i16_expression", test_complex_i16_expression},
        {"complex_i32_expression", test_complex_i32_expression},
    );
    
    printf("All compiler level 2 tests passed!)\n");
    return 0;
}
