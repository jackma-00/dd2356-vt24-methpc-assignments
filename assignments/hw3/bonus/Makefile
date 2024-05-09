CC = cc
TARGET = dmm
SRCS = dmm.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
