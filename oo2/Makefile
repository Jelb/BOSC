all: sum

OBJS = oo2_1.o
LIBS= -lm -pthread
CC = gcc

sum: ${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS} 

clean:
	rm -rf *o sum
