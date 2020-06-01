TARGET = origin
CC = gcc
YY = yacc
CFLAGS = -c -g -Wall -Wswitch-enum -pedantic -DDEBUG
OBJS = lex.yy.o y.tab.o main.o interface.o create.o execute.o\
	   eval.o string.o string_pool.o util.o native.o mem.o bignum.o
INCLUDES = \

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $@ -lm

y.tab.h:Parser/origin.y
	$(YY) -dv Parser/origin.y

y.tab.c:Parser/origin.y
	$(YY) -dv Parser/origin.y

lex.yy.c:Parser/origin.l Parser/origin.y y.tab.h
	lex Parser/origin.l

y.tab.o:y.tab.c origin.h MEM.h
	$(CC) -c $*.c $(INCLUDES)

lex.yy.o:lex.yy.c origin.h MEM.h y.tab.h
	$(CC) -c -g $*.c $(INCLUDES)

bignum.o:native_obj/bignum/bignum.c native_obj/bignum/bignum.h
	$(CC) -c -g native_obj/bignum/bignum.c

.c.o:
	$(CC) $(CFLAGS) $*.c $(INCLUDES)
	echo "in .c.o rule"

# mem.o
mem.o:memory.o storage.o
	ld -r -o mem.o memory.o storage.o

memory.o:memory.c memory.h MEM.h
storage.o:storage.c memory.h MEM.h

#################################
create.o: create.c MEM.h DBG.h origin.h ORG.h ORG_dev.h
error.o: error.c MEM.h origin.h ORG.h ORG_dev.h
error_message.o: error_message.c origin.h MEM.h ORG.h ORG_dev.h
eval.o: eval.c MEM.h DBG.h origin.h ORG.h ORG_dev.h
execute.o: execute.c MEM.h DBG.h origin.h ORG.h ORG_dev.h
interface.o: interface.c MEM.h DBG.h origin.h ORG.h ORG_dev.h
main.o: main.c ORG.h MEM.h
native.o: native.c MEM.h DBG.h origin.h ORG.h ORG_dev.h
string.o: string.c MEM.h origin.h ORG.h ORG_dev.h
string_pool.o: string_pool.c MEM.h DBG.h origin.h ORG.h ORG_dev.h
util.o: util.c MEM.h DBG.h origin.h ORG.h ORG_dev.h



clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h $(TARGET) *~

