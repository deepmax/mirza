CC = cc
CFLAGS = -g -Wall -fdiagnostics-color=always -I..
BUILD = build/
LIBS = -lm

.PHONY: default all clean test test-all test-vector test-list test-buffer

# Find all test source files
TEST_SOURCES = $(wildcard test_*.c)
TEST_TARGETS = $(patsubst test_%.c, $(BUILD)/test_%, $(TEST_SOURCES))

# Source files to test (from parent directory)
SOURCE_FILES = ../vector.c ../list.c ../buffer.c

# Compiler source files
COMPILER_SOURCES = ../ast.c ../buffer.c ../context.c ../jump.c ../lexer.c ../list.c ../panic.c ../parser.c ../vector.c ../vm.c
COMPILER_OBJECTS = $(patsubst ../%.c, $(BUILD)/%.o, $(COMPILER_SOURCES))

# Test helper source
TEST_HELPER_SOURCES = tests.c
TEST_HELPER_OBJECTS = $(patsubst %.c, $(BUILD)/%.o, $(TEST_HELPER_SOURCES))

default: test-all

# Build all test executables
all: $(TEST_TARGETS)

# Build object files for source files being tested
$(BUILD)/vector.o: ../vector.c ../vector.h
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/list.o: ../list.c ../list.h
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/buffer.o: ../buffer.c ../buffer.h ../types.h
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Build compiler object files
$(BUILD)/ast.o: ../ast.c ../ast.h
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/context.o: ../context.c ../context.h
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/jump.o: ../jump.c ../jump.h
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/lexer.o: ../lexer.c ../lexer.h
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/panic.o: ../panic.c ../panic.h
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/parser.o: ../parser.c ../parser.h
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/vm.o: ../vm.c ../vm.h
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Build test executables
$(BUILD)/test_vector: test_vector.c $(BUILD)/vector.o
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< $(BUILD)/vector.o $(LIBS) -o $@

$(BUILD)/test_list: test_list.c $(BUILD)/list.o
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< $(BUILD)/list.o $(LIBS) -o $@

$(BUILD)/test_buffer: test_buffer.c $(BUILD)/buffer.o
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< $(BUILD)/buffer.o $(LIBS) -o $@

# Build test helper object
$(BUILD)/tests.o: tests.c tests.h
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Build compiler test executables
$(BUILD)/test_basics: test_basics.c $(COMPILER_OBJECTS) $(BUILD)/tests.o
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< $(COMPILER_OBJECTS) $(BUILD)/tests.o $(LIBS) -o $@

$(BUILD)/test_int: test_int.c $(COMPILER_OBJECTS) $(BUILD)/tests.o
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< $(COMPILER_OBJECTS) $(BUILD)/tests.o $(LIBS) -o $@

# Run all tests
test-all: all
	@echo "Running all tests..."
	@failed=0; \
	for test in $(TEST_TARGETS); do \
		echo ""; \
		echo "=== Running $$(basename $$test) ==="; \
		$$test || failed=1; \
	done; \
	if [ $$failed -eq 1 ]; then \
		echo ""; \
		echo "Some tests failed!"; \
		exit 1; \
	else \
		echo ""; \
		echo "All tests passed!"; \
	fi

# Run a specific test or all tests
# Usage: make test (runs all) or make test vector (runs test_vector)
test:
	@args="$(filter-out test,$(MAKECMDGOALS))"; \
	if [ -z "$$args" ] || [ "$$args" = "all" ]; then \
		$(MAKE) test-all; \
	else \
		unit=$$args; \
		$(MAKE) $(BUILD)/test_$$unit && $(BUILD)/test_$$unit; \
	fi

# Individual test targets for convenience
test-vector: $(BUILD)/test_vector
	$(BUILD)/test_vector

test-list: $(BUILD)/test_list
	$(BUILD)/test_list

test-buffer: $(BUILD)/test_buffer
	$(BUILD)/test_buffer

test-basics: $(BUILD)/test_basics
	$(BUILD)/test_basics

test-int: $(BUILD)/test_int
	$(BUILD)/test_int

# Prevent make from trying to build arguments as targets
%:
	@:

clean:
	-rm -f -r $(BUILD)
	-rm -f $(TEST_TARGETS)

