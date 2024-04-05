CC = cc
CFLAGS = -O2

SRCS = transpose.c
OBJS = $(SRCS:.c=.o)
TARGET = transpose.out

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