all: fifo

OBJS = list.o main.o 
LIBS= -lpthread -pthread 

OBJSPC = list.o oo2_3.o

fifo: main.o ${OBJS}
	gcc -o $@  ${LIBS} ${OBJS}

procon: oo2_3.o ${OBJSPC}
	gcc -o $@ ${LIBS} ${OBJSPC}

clean:
	rm -rf *o fifo
	rm -rf *o procon
