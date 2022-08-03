#ifndef TRAVERSE_H
#define TRAVERSE_H

#include "vector.h"
#include "graph_node.h"


typedef struct search_node search_node_t;
typedef struct path path_t;


struct search_node {

	graph_node_t * node;
	graph_node_t * prev_search_node;

	int64_t cost;
	uint8_t visited;

};


struct path {

	uint64_t start_id;
	uint64_t end_id;

	vector_t nodes_stack;
};


#endif
