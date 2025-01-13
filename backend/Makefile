# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -I.

# Libraries
LIBS = -lmicrohttpd -lcurl -ljson-c

# Source files
SRCS = server.c ai.c linked_list.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = server

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
