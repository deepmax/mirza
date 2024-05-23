CC = cc
TARGET = $(BUILD)/prog
LIBS = -lm
BUILD= build/
CFLAGS = -g -Wall -fdiagnostics-color=always

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, $(BUILD)/%.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

$(BUILD)/%.o: %.c $(HEADERS)
	mkdir -p ${dir $@}
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	-rm -f -r $(BUILD)
	-rm -f $(TARGET)
