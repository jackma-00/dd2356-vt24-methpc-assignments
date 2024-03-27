CC = cc
CFLAGS = -Wall -Wextra -g

SRCS = helloworld.c
OBJS = $(SRCS:.c=.o)
TARGET = myprogram.out

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