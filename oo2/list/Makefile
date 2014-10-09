all: fifo

OBJS = list.o main.o 
LIBS= -pthread 

OBJSPC = list.o oo2_3.o


fifo: main.o ${OBJS}
	gcc -o $@  ${LIBS} ${OBJS}
	
clean:
	rm -rf *o fifo procon

