TARGET = origin
CC = gcc
YY = yacc
CFLAGS = -c -g -Wall -Wswitch-enum -pedantic -DDEBUG
OBJS = 	\
		lex.yy.o \
		y.tab.o	\
		main.o\
		Origin/interface.o\
		Origin/create.o \
		Origin/execute.o\
		Origin/eval.o \
		string.o \
		string_pool.o \
		Origin/util.o \
		Origin/native.o \
		mem.o bignum.o

INCLUDES = -I. -I./Include
COREDIR = Origin

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $@ -lm

y.tab.h:Parser/origin.y
	$(YY) -dv Parser/origin.y

y.tab.c:Parser/origin.y
	$(YY) -dv Parser/origin.y

lex.yy.c:Parser/origin.l Parser/origin.y y.tab.h
	lex Parser/origin.l

y.tab.o:y.tab.c Include/origin.h Include/MEM.h
	$(CC) -c $*.c $(INCLUDES)

lex.yy.o:lex.yy.c Include/origin.h Include/MEM.h y.tab.h
	$(CC) -c -g $*.c $(INCLUDES)

bignum.o:native_obj/bignum/bignum.c native_obj/bignum/bignum.h
	$(CC) -c -g native_obj/bignum/bignum.c

.c.o:
	$(CC) $(CFLAGS) $*.c $(INCLUDES) -o $@
	echo "in .c.o rule"

# mem.o
mem.o:Origin/memory.o Origin/storage.o
	ld -r -o $@ Origin/memory.o Origin/storage.o

memory.o:Origin/memory.c Include/memory.h Include/MEM.h
storage.o:Origin/storage.c Include/memory.h Include/MEM.h

#################################
create.o: Origin/create.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h
error.o: error.c Include/MEM.h Include/origin.h Include/ORG.h Include/ORG_dev.h
error_message.o: error_message.c Include/origin.h Include/MEM.h Include/ORG.h Include/ORG_dev.h
eval.o: Origin/eval.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h
execute.o: Origin/execute.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h
interface.o: Origin/interface.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h
main.o: main.c Include/ORG.h Include/MEM.h
native.o: Origin/native.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h
string.o: string.c Include/MEM.h Include/origin.h Include/ORG.h Include/ORG_dev.h
string_pool.o: string_pool.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h
util.o: Origin/util.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h



clean:
	rm -f *.o Origin/*.o lex.yy.c y.tab.c y.tab.h $(TARGET) *~

