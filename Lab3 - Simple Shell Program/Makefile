CC = gcc

CFLAGS = -Wall -g 

TARGET = myshell

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) myshell.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)	