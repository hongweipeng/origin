TARGET = origin
CC = gcc
YY = yacc
CFLAGS = -c -g -Wall -Wswitch-enum -pedantic -DDEBUG
OBJS = lex.yy.o y.tab.o main.o create.o eval.o

INCLUDES = \

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $@ -lm

y.tab.h:origin.y
	$(YY) -dv origin.y

y.tab.c:origin.y
	$(YY) -dv origin.y

lex.yy.c:origin.l origin.y y.tab.h
	lex origin.l

y.tab.o:y.tab.c origin.h
	$(CC) -c $*.c $(INCLUDES)

lex.yy.o:lex.yy.c origin.h y.tab.h
	$(CC) -c -g $*.c $(INCLUDES)

.c.o:
	$(CC) $(CFLAGS) $*.c $(INCLUDES)
	echo "in .c.o rule"

create.o: create.c origin.h
main.o: main.c
eval.o: eval.c origin.h

clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h $(TARGET) *~