all: banker

OBJS = jonasbanker.o
LIBS= -lm -pthread
CC = gcc

banker: ${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS} 

clean:
	rm -rf *o jonasbanker
