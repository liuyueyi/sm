OBJS = main.o config.o rsa.o encrypt.o
CC = gcc
CFLAGS = -Wall -O -g
kmc : $(OBJS)
	$(CC) $^ -lcrypto -o $@

.PHONY: clean	
clean:
	-rm kmc $(OBJS) *.*~
