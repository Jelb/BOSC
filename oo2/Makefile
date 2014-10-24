all: sum procon

PCOBJS  = oo2_3.o list/list.o
SUMOBJS = oo2_1.o
LIBS    = -lm -pthread
CC      = gcc

sum: ${SUMOBJS}
	${CC} -o $@ ${SUMOBJS} ${LIBS} 
	
procon: ${PCOBJS}
	${CC} -o $@ ${LIBS} ${PCOBJS}


clean:
	rm -rf *o sum procon
