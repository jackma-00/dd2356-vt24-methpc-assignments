CC = mpicc
NOFProcs = 4
TARGET = hello_mpi
SRCS = hello_mpi.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $< -o $@

run: $(TARGET)
	mpirun -n $(NOFProcs) ./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)