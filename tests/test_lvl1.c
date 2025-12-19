#include "tests.h"
#include "../parser.h"
#include "../vm.h"
#include "../context.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// Helper to capture stdout
static char captured_output[4096];
static int original_stdout_fd;
static int stdout_pipe[2];

static void capture_stdout_start(void)
{
    if (pipe(stdout_pipe) != 0) {
        perror("pipe");
        exit(1);
    }
    fflush(stdout);
    original_stdout_fd = dup(STDOUT_FILENO);
    if (original_stdout_fd < 0) {
        perror("dup");
        exit(1);
    }
    if (dup2(stdout_pipe[1], STDOUT_FILENO) < 0) {
        perror("dup2");
        exit(1);
    }
    close(stdout_pipe[1]);
    setvbuf(stdout, NULL, _IONBF, 0);  // Unbuffered mode
}

static void capture_stdout_end(void)
{
    fflush(stdout);
    close(STDOUT_FILENO);
    
    // Clear the buffer first
    memset(captured_output, 0, sizeof(captured_output));
    
    ssize_t bytes_read = read(stdout_pipe[0], captured_output, sizeof(captured_output) - 1);
    if (bytes_read < 0) {
        perror("read");
        exit(1);
    }
    if (bytes_read > 0) {
        captured_output[bytes_read] = '\0';
    }
    close(stdout_pipe[0]);
    
    // Restore original stdout
    if (dup2(original_stdout_fd, STDOUT_FILENO) < 0) {
        perror("dup2 restore");
        exit(1);
    }
    close(original_stdout_fd);
}

static void reset_compiler_state(void)
{
    // Clear global context symbols
    // Note: global_context is a const pointer, but the struct it points to is mutable
    context_t* ctx = (context_t*)global_context;  // Cast away const for field access
    if (ctx->symbols != NULL) {
        // Free all symbols in the vector
        for (size_t i = 0; i < vec_size(ctx->symbols); i++) {
            symbol_t* sym = (symbol_t*) vec_get(ctx->symbols, i);
            free(sym);
        }
        vec_free(ctx->symbols);
        ctx->symbols = NULL;
    }
    ctx->allocated = 0;
}

static void compile_and_run(const char* code)
{
    // Reset state before each test
    reset_compiler_state();
    
    // Create a temporary file
    char tmp_filename[] = "/tmp/mirza_test_XXXXXX";
    int fd = mkstemp(tmp_filename);
    if (fd < 0) {
        perror("mkstemp");
        exit(1);
    }
    
    // Write code to the file
    ssize_t written = write(fd, code, strlen(code));
    if (written < 0) {
        perror("write");
        close(fd);
        unlink(tmp_filename);
        exit(1);
    }
    close(fd);
    
    // Parse and execute from file
    parser_load(tmp_filename);
    parser_start(1, 0);  // execute=true, dasm=false
    parser_free();
    
    // Free VM after execution
    vm_free();
    
    // Clean up
    unlink(tmp_filename);
}

// Test 1: Simple integer constant
static void test_simple_constant(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 42\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

// Test 2: Simple arithmetic - addition
static void test_simple_addition(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 10 + 20\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "30", "should print 30");
}

// Test 3: Simple arithmetic - subtraction
static void test_simple_subtraction(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 50 - 20\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "30", "should print 30");
}

// Test 4: Simple arithmetic - multiplication
static void test_simple_multiplication(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 6 * 7\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

// Test 5: Simple arithmetic - division
static void test_simple_division(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 20 / 4\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "5", "should print 5");
}

// Test 6: Simple arithmetic - modulo
static void test_simple_modulo(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 17 % 5\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2", "should print 2");
}

// Test 7: Variable declaration and assignment
static void test_variable_declaration(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x = 42\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

// Test 8: Variable reassignment
static void test_variable_reassignment(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x = 10\nx = 20\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "20", "should print 20");
}

// Test 9: Multiple variables
static void test_multiple_variables(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var a = 5\nvar b = 10\nprint a + b\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "15", "should print 15");
}

// Test 10: Complex arithmetic expression
static void test_complex_arithmetic(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 2 + 3 * 4\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "14", "should respect operator precedence (2 + 12 = 14)");
}

// Test 11: Parentheses in expressions
static void test_parentheses(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print (2 + 3) * 4\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "20", "should respect parentheses ((2+3)*4 = 20)");
}

// Test 12: Comparison operators - less than
static void test_comparison_lt(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 5 < 10\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (true)");
}

// Test 13: Comparison operators - greater than
static void test_comparison_gt(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 10 > 5\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (true)");
}

// Test 14: Comparison operators - equality
static void test_comparison_eq(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 5 == 5\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (true)");
}

// Test 15: Comparison operators - inequality
static void test_comparison_ne(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 5 != 10\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (true)");
}

// Test 16: If statement - true condition
static void test_if_true(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("if 1 {\nprint 42\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

// Test 17: If statement - false condition
static void test_if_false(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("if 0 {\nprint 42\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "", "should print nothing");
}

// Test 18: If-else statement
static void test_if_else(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("if 0 {\nprint 1\n} else {\nprint 2\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2", "should print 2");
}

// Test 19:


// Test 20: For loop - simple
static void test_for_simple(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i = 0; i < 3; i = i + 1 {\nprint i\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "012", "should print 012");
}

// Test 21: Break statement
static void test_break_statement(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i = 0; i < 5; i = i + 1 {\nprint i\nif i == 2 {\nbreak\n}\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "012", "should print 012 and break");
}

// Test 22: Continue statement
static void test_continue_statement(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i = 0; i < 5; i = i + 1 {\nif i == 2 {\ncontinue\n}\nprint i\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "0134", "should skip 2");
}

// Test 23: Simple function - no parameters
static void test_function_no_params(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func hello() : void {\nprint 42\n}\nhello()\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

// Test 24: Simple function - with parameters
static void test_function_with_params(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func add(a : int, b : int) : int {\nret a + b\n}\nprint add(5, 3)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "8", "should print 8");
}

// Test 25: Function with return value
static void test_function_return(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func square(x : int) : int {\nret x * x\n}\nprint square(6)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "36", "should print 36");
}

// Test 26: Recursive function - factorial
static void test_recursive_factorial(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func fact(n : int) : int {\nif n <= 1 {\nret 1\n}\nret fact(n - 1) * n\n}\nprint fact(5)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "120", "should print 120 (5!)");
}

// Test 27: Nested if statements
static void test_nested_if(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x = 10\nif x > 5 {\nif x < 15 {\nprint 42\n}\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "42", "should print 42");
}

// Test 28: Nested loops
static void test_nested_loops(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i = 0; i < 2; i = i + 1 {\nfor var j = 0; j < 2; j = j + 1 {\nprint i * 10 + j\n}\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "011011", "should print 011011");
}

// Test 29: Logical AND
static void test_logical_and(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 1 and 1\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (true)");
}

// Test 30: Logical OR
static void test_logical_or(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 0 or 1\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (true)");
}

// Test 31: Unary minus
static void test_unary_minus(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print -5\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "-5", "should print -5");
}

// Test 32: String literal
static void test_string_literal(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print \"hello\"\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "hello", "should print hello");
}

// Test 33: Multiple print statements
static void test_multiple_prints(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 1\nprint 2\nprint 3\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "123", "should print 123");
}

// Test 34: Complex expression with all operators
static void test_complex_expression(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print (10 + 5) * 2 - 3\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "27", "should print 27");
}

// Test 35: Function call in expression
static void test_function_in_expression(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func double(x : int) : int {\nret x * 2\n}\nprint double(5) + 10\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "20", "should print 20");
}

// Test 36: Variable scope in function
static void test_variable_scope(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("var x = 10\nfunc test() : void {\nvar x = 20\nprint x\n}\ntest()\nprint x\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "2010", "should print 2010");
}

// Test 37: Multiple function calls
static void test_multiple_function_calls(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("func inc(x : int) : int {\nret x + 1\n}\nprint inc(1)\nprint inc(2)\nprint inc(3)\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "234", "should print 234");
}

// Test 38: ....

// Test 39: For loop with step
static void test_for_with_step(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i = 0; i < 10; i = i + 2 {\nprint i\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "02468", "should print 02468");
}

// Test 40: Complex nested control flow
static void test_complex_nested_control(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("for var i = 0; i < 3; i = i + 1 {\nif i == 1 {\nprint 10\n} else {\nprint 20\n}\n}\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "201020", "should print 201020");
}

// Test 41: Bitwise AND
static void test_bitwise_and(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 5 & 3\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "1", "should print 1 (5 & 3 = 1)");
}

// Test 42: Bitwise OR
static void test_bitwise_or(test_suite_t* suite)
{
    capture_stdout_start();
    compile_and_run("print 5 | 3\n");
    capture_stdout_end();
    
    TEST_ASSERT_STR_EQ(captured_output, "7", "should print 7 (5 | 3 = 7)");
}

// Test 43: Bitwise XOR
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

