#include "tests.h"


static void test_simple_constant(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 42\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

static void test_simple_addition(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 10 + 20\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "30", "should print 30");
}

static void test_simple_subtraction(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 50 - 20\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "30", "should print 30");
}

static void test_simple_multiplication(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 6 * 7\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

static void test_simple_division(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 20 / 4\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "5", "should print 5");
}

static void test_simple_modulo(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 17 % 5\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2", "should print 2");
}

static void test_variable_declaration(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x = 42\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

static void test_variable_reassignment(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x = 10\nx = 20\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "20", "should print 20");
}

static void test_multiple_variables(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a = 5\nvar b = 10\nprint a + b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "15", "should print 15");
}

static void test_complex_arithmetic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 2 + 3 * 4\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "14", "should respect operator precedence (2 + 12 = 14)");
}

static void test_parentheses(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print (2 + 3) * 4\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "20", "should respect parentheses ((2+3)*4 = 20)");
}

static void test_comparison_lt(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 5 < 10\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (true)");
}

static void test_comparison_gt(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 10 > 5\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (true)");
}

static void test_comparison_eq(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 5 == 5\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (true)");
}

static void test_comparison_ne(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 5 != 10\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (true)");
}

static void test_if_true(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("if 1 {\nprint 42\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

static void test_if_false(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("if 0 {\nprint 42\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "", "should print nothing");
}

static void test_if_else(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("if 0 {\nprint 1\n} else {\nprint 2\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2", "should print 2");
}

static void test_for_simple(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i = 0; i < 3; i = i + 1 {\nprint i\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "012", "should print 012");
}

static void test_break_statement(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i = 0; i < 5; i = i + 1 {\nprint i\nif i == 2 {\nbreak\n}\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "012", "should print 012 and break");
}

static void test_continue_statement(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i = 0; i < 5; i = i + 1 {\nif i == 2 {\ncontinue\n}\nprint i\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "0134", "should skip 2");
}

static void test_function_no_params(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func hello() : void {\nprint 42\n}\nhello()\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

static void test_function_with_params(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func add(a : i32, b : i32) : void {\nret a + b\n}\nprint add(5, 3)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "8", "should print 8");
}

static void test_function_return(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func square(x : i32) : i32 {\nret x * x\n}\nprint square(6)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "36", "should print 36");
}

static void test_recursive_factorial(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func fact(n : i32) : i32 {\nif n <= 1 {\nret 1\n}\nret fact(n - 1) * n\n}\nprint fact(5)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "120", "should print 120 (5!)");
}

static void test_nested_if(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x = 10\nif x > 5 {\nif x < 15 {\nprint 42\n}\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

static void test_nested_loops(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i = 0; i < 2; i = i + 1 {\nfor var j = 0; j < 2; j = j + 1 {\nprint i * 10 + j\n}\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "011011", "should print 011011");
}

static void test_logical_and(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 1 and 1\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (true)");
}

static void test_logical_or(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 0 or 1\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (true)");
}

static void test_unary_minus(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print -5\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-5", "should print -5");
}

static void test_string_literal(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print \"hello\"\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "hello", "should print hello");
}

static void test_utf8_string_basic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print \"Hello 世界\"\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "Hello 世界", "should print UTF-8 string with Chinese characters");
}

static void test_utf8_string_emoji(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print \"Hello 🌍\"\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "Hello 🌍", "should print UTF-8 string with emoji");
}

static void test_utf8_string_mixed(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print \"Привет мир! こんにちは\"\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "Привет мир! こんにちは", "should print UTF-8 string with Cyrillic and Japanese");
}

static void test_utf8_string_variable(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var msg = \"Hello 世界\"\nprint msg\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "Hello 世界", "should store and print UTF-8 string from variable");
}

static void test_utf8_string_reassign(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var s = \"hello\"\ns = \"世界\"\nprint s\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "世界", "should reassign UTF-8 string to variable");
}

static void test_utf8_string_multibyte(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print \"€\"\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "€", "should print 3-byte UTF-8 character (Euro sign)");
}

static void test_multiple_prints(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 1\nprint 2\nprint 3\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "123", "should print 123");
}

static void test_complex_expression(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print (10 + 5) * 2 - 3\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "27", "should print 27");
}

static void test_function_in_expression(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func double(x : i32) : i32 {\nret x * 2\n}\nprint double(5) + 10\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "20", "should print 20");
}

static void test_variable_scope(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x = 10\nfunc test() : void {\nvar x = 20\nprint x\n}\ntest()\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2010", "should print 2010");
}

static void test_multiple_function_calls(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func inc(x : i32) : i32 {\nret x + 1\n}\nprint inc(1)\nprint inc(2)\nprint inc(3)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "234", "should print 234");
}

static void test_for_with_step(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i = 0; i < 10; i = i + 2 {\nprint i\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "02468", "should print 02468");
}

static void test_complex_nested_control(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i = 0; i < 3; i = i + 1 {\nif i == 1 {\nprint 10\n} else {\nprint 20\n}\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "201020", "should print 201020");
}

static void test_bitwise_and(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 5 & 3\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (5 & 3 = 1)");
}

static void test_bitwise_or(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 5 | 3\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "7", "should print 7 (5 | 3 = 7)");
}

static void test_bitwise_xor(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 5 ^ 3\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "6", "should print 6 (5 ^ 3 = 6)");
}

int main(void)
{
    RUN_SUITE("compiler level 1",
        {"simple_constant", test_simple_constant},
        {"simple_addition", test_simple_addition},
        {"simple_subtraction", test_simple_subtraction},
        {"simple_multiplication", test_simple_multiplication},
        {"simple_division", test_simple_division},
        {"simple_modulo", test_simple_modulo},
        {"variable_declaration", test_variable_declaration},
        {"variable_reassignment", test_variable_reassignment},
        {"multiple_variables", test_multiple_variables},
        {"complex_arithmetic", test_complex_arithmetic},
        {"parentheses", test_parentheses},
        {"comparison_lt", test_comparison_lt},
        {"comparison_gt", test_comparison_gt},
        {"comparison_eq", test_comparison_eq},
        {"comparison_ne", test_comparison_ne},
        {"if_true", test_if_true},
        {"if_false", test_if_false},
        {"if_else", test_if_else},
        {"for_simple", test_for_simple},
        {"break_statement", test_break_statement},
        {"continue_statement", test_continue_statement},
        {"function_no_params", test_function_no_params},
        {"function_with_params", test_function_with_params},
        {"function_return", test_function_return},
        {"recursive_factorial", test_recursive_factorial},
        {"nested_if", test_nested_if},
        {"nested_loops", test_nested_loops},
        {"logical_and", test_logical_and},
        {"logical_or", test_logical_or},
        {"unary_minus", test_unary_minus},
        {"string_literal", test_string_literal},
        {"utf8_string_basic", test_utf8_string_basic},
        {"utf8_string_emoji", test_utf8_string_emoji},
        {"utf8_string_mixed", test_utf8_string_mixed},
        {"utf8_string_variable", test_utf8_string_variable},
        {"utf8_string_reassign", test_utf8_string_reassign},
        {"utf8_string_multibyte", test_utf8_string_multibyte},
        {"multiple_prints", test_multiple_prints},
        {"complex_expression", test_complex_expression},
        {"function_in_expression", test_function_in_expression},
        {"variable_scope", test_variable_scope},
        {"multiple_function_calls", test_multiple_function_calls},
        {"for_with_step", test_for_with_step},
        {"complex_nested_control", test_complex_nested_control},
        {"bitwise_and", test_bitwise_and},
        {"bitwise_or", test_bitwise_or},
        {"bitwise_xor", test_bitwise_xor}
    );
    
    printf("All compiler level 1 tests passed!\n");
    return 0;
}

