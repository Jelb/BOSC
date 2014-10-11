all: sum procon

SOBJS = oo2_1.o
LIBS= -lm -pthread
CC = gcc

POBJS = list/list.o oo2_3.o

sum: ${SOBJS}
	${CC} -o $@ ${SOBJS} ${LIBS} 
	
procon: oo2_3.o list/list.o ${OBJSPC}
	gcc -o $@ ${LIBS} ${POBJS}
	

clean:
	rm -rf *o sum
	rm -rf *o procon
