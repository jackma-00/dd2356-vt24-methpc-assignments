CC = mpicc
NOFProcs = 2
TARGET = ping_pong
SRCS = ping_pong.c
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
