CC=gcc
CFLAGS= -Wall -ggdb -O0
LIBS=
CLEAN_TARGETS=test main.o debug.o debug.h.gch graph.o graph.h.gch graph_node.o graph_node.h.gch vector.o vector.h.gch error.h.gch

test: main.o debug.o debug.h graph.o graph.h graph_node.o graph_node.h vector.o vector.h error.h
	${CC} ${CFLAGS} -o test main.o debug.o debug.h graph.o graph.h graph_node.o graph_node.h vector.o vector.h error.h

main.o: main.c graph.h graph_node.h vector.h error.h
	${CC} ${CFLAGS} -c main.c graph.h graph_node.h vector.h error.h

debug.o: debug.c graph.h graph_node.h vector.h error.h
	${CC} ${CFLAGS} -c debug.c debug.h graph.h graph_node.h vector.h error.h

graph.o: graph.c graph.h graph_node.h vector.h error.h
	${CC} ${CFLAGS} -c graph.c graph.h graph_node.h vector.h error.h

graph_node.o: graph_node.c graph_node.h vector.h error.h
	${CC} ${CFLAGS} -c graph_node.c graph_node.h vector.h error.h

vector.o: vector.c vector.h error.h
	${CC} ${CFLAGS} -c vector.c vector.h error.h

clean:
	rm ${CLEAN_TARGETS}
