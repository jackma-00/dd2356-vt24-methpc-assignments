export OMP_NUM_THREADS=8

CC = cc
TARGET = myexecutable.out
SRCS = main.c activematter_omp.c
OBJS = $(SRCS:.c=.o)

PHONY: all clean stat

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -fopenmp $^ -lm -o $@

%.o: %.c
	$(CC) -fopenmp -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

stat: 
	perf stat ./$(TARGET)
