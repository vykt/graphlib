#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <sys/types.h>
#include <stdint.h>

#include "vector.h"


#define GRAPH_NODE_NOT_VISITED 0
#define GRAPH_NODE_VISITED 1


typedef struct graph_node graph_node_t;

struct graph_node {

	char * data;      //pointer to heap space
	size_t data_size; //size of heap space

	vector_t neighbours;  //uint64_t IDs of neighbours.
	vector_t edge_weights; //int64_t, weighs to get to neighbour nodes

	uint64_t id;
	uint8_t visited;

};


int graph_node_set_dat(graph_node_t * n, char * data, size_t data_size);
int graph_node_get_dat(graph_node_t * n, char ** data);

int graph_node_add_nbr(graph_node_t * n, uint64_t n_add_id, int64_t weight);
int graph_node_rmv_nbr(graph_node_t * n, uint64_t n_rmv_id);
int graph_node_get_nbr_id(graph_node_t * n, uint64_t index, uint64_t * nbr, int64_t * w);

int graph_node_ini(graph_node_t * n, char * data, size_t data_size, uint64_t id);
int graph_node_end(graph_node_t * n);


#endif
