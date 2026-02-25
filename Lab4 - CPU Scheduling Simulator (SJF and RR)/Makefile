CC = gcc

CFLAGS = -Wall -g 

TARGET = simulator

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) simulator.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)	