CC = cc
TARGET = myprogram
SRCS = test_fox.c
OBJS = $(SRCS:.c=.o)
HDRS = fox.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

%.o: %.c $(HDRS)
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)