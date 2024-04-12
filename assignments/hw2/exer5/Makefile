CC ?= gcc
CFLAGS = -Wall -O2 -fopenmp -DDEBUG
TARGET = DFTW_1.out
SRCS = DFTW_1.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)