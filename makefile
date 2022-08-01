CC=gcc
CFLAGS= -Wall -ggdb -O0
LIBS=
CLEAN_TARGETS=test DEBUG.o graph.o graph.h.gch graph_node.o graph_node.h.gch vector.o vector.h.gch error.h.gch

test: DEBUG.o graph.o graph.h graph_node.o graph_node.h vector.o vector.h error.h
	${CC} ${CFLAGS} -o test DEBUG.o graph.o graph.h graph_node.o graph_node.h vector.o vector.h error.h

DEBUG.o: DEBUG.c graph.h graph_node.h vector.h error.h
	${CC} ${CFLAGS} -c DEBUG.c graph.h graph_node.h vector.h error.h

graph.o: graph.c graph.h graph_node.h vector.h error.h
	${CC} ${CFLAGS} -c graph.c graph.h graph_node.h vector.h error.h

graph_node.o: graph_node.c graph_node.h vector.h error.h
	${CC} ${CFLAGS} -c graph_node.c graph_node.h vector.h error.h

vector.o: vector.c vector.h error.h
	${CC} ${CFLAGS} -c vector.c vector.h error.h

clean:
	rm ${CLEAN_TARGETS}
