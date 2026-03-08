# Define the c compiler
CC = gcc

# Define any compile-time flags 
CFLAGS = -Wall -g 

# Define the target executable name
TARGET=pages
#  'all' is the default target
all: $(TARGET)

# The target will be rebuilt when 'make' is run
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
