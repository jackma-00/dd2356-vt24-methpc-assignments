CC = cc
CFLAGS = -g -O2

SRCS = matrix_multiply.c
OBJS = $(SRCS:.c=.o)
TARGET = matrix_multiply.out

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)