# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Files
SRCS = Guillermo_libFC.c Guillermo_testapp.c
OBJS = $(SRCS:.c=.o)

# Output executable
TARGET = Guillermo_testapp

# Default target
all: $(TARGET)

# Build executable from object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile .c files into .o files
%.o: %.c Guillermo_libFC.h
	$(CC) $(CFLAGS) -c $< -o $@

