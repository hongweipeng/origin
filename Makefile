TARGET = origin
CC = gcc
CFLAGS = -c -g -Wall -Wswitch-enum -ansi -pedantic -DDEBUG
OBJS = lex.yy.o y.tab.o 


# mem.o
	





clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h *~

