OBJS = main.o config.o rsa.o
CC = gcc
CFLAGS = -Wall -O -g -lcrypto
kmc : $(OBJS)
	$(CC) $^ -o $@	
	
clean:
	rm kmc $(OBJS)
