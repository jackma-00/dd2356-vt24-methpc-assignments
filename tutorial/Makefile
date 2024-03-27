CC = cc
CFLAGS = -Wall -Wextra -g

SRCS = hello.c
OBJS = $(SRCS:.c=.o)
TARGET = hello.out

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)