OBJS = main.o config.o
CC = gcc
CFLAGS = -Wall -O -g
kmc : $(OBJS)
	$(CC) $^ -o $@
	
clean:
	rm kmc $(OBJS)
