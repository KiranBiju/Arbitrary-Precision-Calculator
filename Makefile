CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = APC.out

SOURCES = main.c arithmetic_functions.c sign.c

all: $(TARGET)

$(TARGET): $(SOURCES) apc.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
