CC = mpicc
NOFProcs = 2
TARGET = pi_seq
SRCS = pi_seq.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $< -o $@

run: $(TARGET)
	mpiexec -n $(NOFProcs) ./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
