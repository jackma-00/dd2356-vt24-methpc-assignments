CC = cc
TARGET = myprogram
#SRCS = dmm.c fox.c
SRCS = test_fox.c fox.c
OBJS = $(SRCS:.c=.o)
HDRS = fox.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

%.o: %.c $(HDRS)
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)