#include "tests.h"

// Helper to capture stdout
char captured_output[4096];
static int original_stdout_fd;
static int stdout_pipe[2];

void capture_stdout_start(void)
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

void capture_stdout_end(void)
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

void reset_compiler_state(void)
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

void compile_and_run(const char* code)
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
    parser_start(1, NULL);  // execute=true, dasm_filename=NULL
    parser_free();
    
    // Free VM after execution
    vm_free();
    
    // Clean up
    unlink(tmp_filename);
}