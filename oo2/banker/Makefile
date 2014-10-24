all: banker

OBJS = banker.o
LIBS = -lm -pthread
CC   = gcc

banker: ${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS} 

clean:
	rm -rf *o banker
