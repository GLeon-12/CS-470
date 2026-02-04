CC = gcc

CFLAGS = -Wall -g 

TARGET = lab2

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) lab2.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)	