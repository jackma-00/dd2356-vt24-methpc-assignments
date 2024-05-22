# On Dardel
#CC = cc
# On my laptop
CC = mpicc
# Installed MPICH
# - From source at https://www.mpich.org/downloads/
# - ./configure, make, make install
TARGET = myexecutable.out
SRCS = main.c activematter_mpi.c
OBJS = $(SRCS:.c=.o)

PHONY: all clean stat

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -lm -o $@

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

stat: 
	perf stat ./$(TARGET)
