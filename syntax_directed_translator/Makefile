CC = gcc
CFLAGS = -Wall

SRCS = src/lexer.c src/parser.c src/main.c src/symtable.c
OBJS = $(SRCS:.c=.o)
TARGET = translator

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
