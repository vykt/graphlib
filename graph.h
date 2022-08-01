#ifndef GRAPH_H
#define GRAPH_H

#include <sys/types.h>
#include <stdint.h>

#include "vector.h"
#include "graph_node.h"


typedef struct graph graph_t;

struct graph {

	//When calling vector_get() on graph, return is a graph_node_t
	vector_t nodes;   //stores graph_node_t *
	uint64_t next_id; //ID to assign to next new node

};


int graph_add(graph_t * g, char * data, size_t data_size); //Add node
int graph_rmv(); //Remove node

int graph_get_dat(graph_t * g, uint64_t index, char ** dat);

int graph_ini(graph_t * g);
int graph_end(graph_t * g);


#endif
