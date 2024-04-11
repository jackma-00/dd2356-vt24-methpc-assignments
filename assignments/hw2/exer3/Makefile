CC ?= gcc
CFLAGS = -fopenmp
MFLAGS = -lm
TARGET = parallel_sum.out
SRCS = parallel_sum.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(MFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(MFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)