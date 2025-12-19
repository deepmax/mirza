#include "tests.h"

// Test signed 8-bit integers
static void test_i8_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i8 = 42\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

static void test_i8_arithmetic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i8 = 10\nvar b : i8 = 20\nprint a + b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "30", "should print 30");
}

static void test_i8_negative(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i8 = -5\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-5", "should print -5");
}

// Test signed 16-bit integers
static void test_i16_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i16 = 1000\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1000", "should print 1000");
}

static void test_i16_arithmetic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i16 = 100\nvar b : i16 = 200\nprint a * b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "20000", "should print 20000");
}

// Test signed 32-bit integers (existing, but verify still works)
static void test_i32_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i32 = 100000\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "100000", "should print 100000");
}

// Test signed 64-bit integers
static void test_i64_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i64 = 2147483647\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2147483647", "should print 2147483647 (max i32)");
}

static void test_i64_large_arithmetic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i64 = 1000000000\nvar b : i64 = 1000000000\nprint a + b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2000000000", "should print 2000000000");
}

// Test unsigned 8-bit integers
static void test_u8_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : u8 = 255\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "255", "should print 255");
}

static void test_u8_arithmetic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : u8 = 100\nvar b : u8 = 50\nprint a + b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "150", "should print 150");
}

// Test unsigned 16-bit integers
static void test_u16_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : u16 = 65535\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "65535", "should print 65535");
}

static void test_u16_arithmetic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : u16 = 1000\nvar b : u16 = 2000\nprint a * b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2000000", "should print 2000000");
}

// Test unsigned 32-bit integers
static void test_u32_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : u32 = 4000000000\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "4000000000", "should print 4000000000");
}

// Test unsigned 64-bit integers
static void test_u64_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : u64 = 2147483647\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2147483647", "should print 2147483647 (max i32, fits in u64)");
}

// Test mixed type operations (should promote to int64)
static void test_mixed_i8_i16(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i8 = 10\nvar b : i16 = 20\nprint a + b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "30", "should print 30 (mixed i8 + i16)");
}

static void test_mixed_i32_i64(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 1000\nvar b : i64 = 2000\nprint a + b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "3000", "should print 3000 (mixed i32 + i64)");
}

static void test_mixed_signed_unsigned(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 100\nvar b : u32 = 200\nprint a + b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "300", "should print 300 (mixed signed + unsigned)");
}

// Test comparisons with different types
static void test_i8_comparison(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i8 = 10\nvar b : i8 = 20\nprint a < b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (10 < 20)");
}

static void test_u16_comparison(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : u16 = 100\nvar b : u16 = 50\nprint a > b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (100 > 50)");
}

// Test unary operations
static void test_i16_unary_minus(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i16 = 100\nprint -x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-100", "should print -100");
}

static void test_i32_unary_minus(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i32 = 500\nprint -x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-500", "should print -500");
}

// Test function parameters with different types
static void test_function_i8_param(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func add(a : i8, b : i8) : void {\nret a + b\n}\nprint add(10, 20)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "30", "should print 30");
}

static void test_function_i16_param(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func multiply(a : i16, b : i16) : i16 {\nret a * b\n}\nprint multiply(10, 5)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "50", "should print 50");
}

static void test_function_u32_param(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func subtract(a : u32, b : u32) : u32 {\nret a - b\n}\nprint subtract(100, 30)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "70", "should print 70");
}

static void test_function_i64_return(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func large() : i64 {\nret 2000000000\n}\nprint large()\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2000000000", "should print 2000000000");
}

// Test complex expressions with different types
static void test_complex_i16_expression(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i16 = 10\nvar b : i16 = 20\nvar c : i16 = 5\nprint (a + b) * c\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "150", "should print 150");
}

static void test_complex_u32_expression(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : u32 = 100\nvar y : u32 = 50\nprint x / y + x % y\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2", "should print 2 (100/50 + 100%50 = 2 + 0)");
}

// Test bitwise operations with different types
static void test_i8_bitwise_and(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i8 = 5\nvar b : i8 = 3\nprint a & b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (5 & 3 = 1)");
}

static void test_u16_bitwise_or(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : u16 = 10\nvar b : u16 = 5\nprint a | b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "15", "should print 15 (10 | 5 = 15)");
}

static void test_i32_bitwise_xor(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i32 = 12\nvar b : i32 = 10\nprint a ^ b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "6", "should print 6 (12 ^ 10 = 6)");
}

// Test assignment with type conversion
static void test_i8_assignment(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i8 = 0\nx = 42\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

static void test_i16_assignment(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i16 = 0\nx = 1000\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1000", "should print 1000");
}

// Test if statements with different integer types
static void test_i8_if_condition(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : i8 = 10\nif x > 5 {\nprint 1\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1");
}

static void test_u16_if_condition(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x : u16 = 100\nif x == 100 {\nprint 1\n} else {\nprint 0\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1");
}

// Test for loops with different integer types
static void test_i8_for_loop(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i : i8 = 0; i < 3; i = i + 1 {\nprint i\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "012", "should print 012");
}

static void test_i16_for_loop(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i : i16 = 0; i < 5; i = i + 1 {\nprint i\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "01234", "should print 01234");
}

// Test multiple variables of different types
static void test_multiple_types(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : i8 = 1\nvar b : i16 = 2\nvar c : i32 = 3\nvar d : i64 = 4\nprint a + b + c + d\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "10", "should print 10");
}

static void test_unsigned_types_mix(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a : u8 = 10\nvar b : u16 = 20\nvar c : u32 = 30\nvar d : u64 = 40\nprint a + b + c + d\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "100", "should print 100");
}

int main(void)
{
    RUN_SUITE("compiler level 2 - integer types",
        // Signed 8-bit
        {"i8_variable", test_i8_variable},
        {"i8_arithmetic", test_i8_arithmetic},
        {"i8_negative", test_i8_negative},
        {"i8_bitwise_and", test_i8_bitwise_and},
        {"i8_assignment", test_i8_assignment},
        {"i8_if_condition", test_i8_if_condition},
        {"i8_for_loop", test_i8_for_loop},
        {"i8_comparison", test_i8_comparison},
        
        // Signed 16-bit
        {"i16_variable", test_i16_variable},
        {"i16_arithmetic", test_i16_arithmetic},
        {"i16_unary_minus", test_i16_unary_minus},
        {"i16_assignment", test_i16_assignment},
        {"i16_for_loop", test_i16_for_loop},
        
        // Signed 32-bit
        {"i32_variable", test_i32_variable},
        {"i32_unary_minus", test_i32_unary_minus},
        {"i32_bitwise_xor", test_i32_bitwise_xor},
        
        // Signed 64-bit
        {"i64_variable", test_i64_variable},
        {"i64_large_arithmetic", test_i64_large_arithmetic},
        
        // Unsigned 8-bit
        {"u8_variable", test_u8_variable},
        {"u8_arithmetic", test_u8_arithmetic},
        
        // Unsigned 16-bit
        {"u16_variable", test_u16_variable},
        {"u16_arithmetic", test_u16_arithmetic},
        {"u16_bitwise_or", test_u16_bitwise_or},
        {"u16_comparison", test_u16_comparison},
        {"u16_if_condition", test_u16_if_condition},
        
        // Unsigned 32-bit
        {"u32_variable", test_u32_variable},
        {"u32_complex_expression", test_complex_u32_expression},
        
        // Unsigned 64-bit
        {"u64_variable", test_u64_variable},
        
        // Mixed type operations
        {"mixed_i8_i16", test_mixed_i8_i16},
        {"mixed_i32_i64", test_mixed_i32_i64},
        {"mixed_signed_unsigned", test_mixed_signed_unsigned},
        {"multiple_types", test_multiple_types},
        {"unsigned_types_mix", test_unsigned_types_mix},
        
        // Functions with different types
        {"function_i8_param", test_function_i8_param},
        {"function_i16_param", test_function_i16_param},
        {"function_u32_param", test_function_u32_param},
        {"function_i64_return", test_function_i64_return},
        
        // Complex expressions
        {"complex_i16_expression", test_complex_i16_expression},
    );
    
    printf("All compiler level 2 tests passed!\n");
    return 0;
}
