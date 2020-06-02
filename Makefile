TARGET = origin
CC = gcc
YY = bison
CFLAGS = -c -g -Wall -Wswitch-enum -pedantic -DDEBUG
OBJS = 	\
		lex.yy.o \
		origin.tab.o	\
		main.o\
		Origin/interface.o\
		Origin/create.o \
		Origin/execute.o\
		Origin/eval.o \
		string.o \
		string_pool.o \
		Origin/util.o \
		Origin/native.o \
		mem.o

INCLUDES = -I. -I./Include
COREDIR = Origin

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $@ -lm

origin.tab.h:Parser/origin.y
	$(YY) -dv Parser/origin.y

origin.tab.c:Parser/origin.y
	$(YY) -dv Parser/origin.y

lex.yy.c:Parser/origin.l Parser/origin.y origin.tab.h
	lex Parser/origin.l

origin.tab.o:origin.tab.c Include/origin.h Include/MEM.h
	$(CC) -c $*.c $(INCLUDES)

lex.yy.o:lex.yy.c Include/origin.h Include/MEM.h origin.tab.h
	$(CC) -c -g $*.c $(INCLUDES)

.c.o:
	$(CC) $(CFLAGS) $*.c $(INCLUDES) -o $@
	echo "in .c.o rule"

# mem.o
mem.o:$(COREDIR)/memory.o $(COREDIR)/storage.o
	ld -r -o $@ $(COREDIR)/memory.o $(COREDIR)/storage.o

memory.o:$(COREDIR)/memory.c Include/memory.h Include/MEM.h
storage.o:$(COREDIR)/storage.c Include/memory.h Include/MEM.h

#################################
create.o: $(COREDIR)/create.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h
error.o: error.c Include/MEM.h Include/origin.h Include/ORG.h Include/ORG_dev.h
error_message.o: error_message.c Include/origin.h Include/MEM.h Include/ORG.h Include/ORG_dev.h
eval.o: $(COREDIR)/eval.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h
execute.o: $(COREDIR)/execute.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h
interface.o: $(COREDIR)/interface.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h
main.o: main.c Include/ORG.h Include/MEM.h
native.o: $(COREDIR)/native.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h
string.o: string.c Include/MEM.h Include/origin.h Include/ORG.h Include/ORG_dev.h
string_pool.o: string_pool.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h
util.o: $(COREDIR)/util.c Include/MEM.h Include/DBG.h Include/origin.h Include/ORG.h Include/ORG_dev.h

rebuild: clean $(TARGET)

clean:
	rm -f *.o $(COREDIR)/*.o lex.yy.c y.tab.c origin.tab.h $(TARGET) $(TARGET).tab.c $(TARGET).tab.h *~

