#ifndef TRAVERSE_H
#define TRAVERSE_H

#include "vector.h"
#include "graph.h"

typedef struct search_node s_node_t;
typedef struct search_graph s_graph_t;
typedef struct path_req path_req_t;


struct search_node {

	graph_node_t * node;
	s_node_t * prev_search_node;

	int64_t cost;
	uint8_t visited;
	uint64_t queue_index; 
};

struct search_graph {

	graph_t * graph;
	vector_t s_queue; //s_node_t *

};

struct path_req {

	uint64_t start_id;
	uint64_t end_id;

	vector_t nodes_stack;
};


int queue_smart_move(vector_t * s_queue, s_node_t * s_node);
int get_index_by_s_node(vector_t * s_queue, s_node_t * s_node, uint64_t * index); 
int get_s_node_by_id(s_graph_t * s_graph, uint64_t id, s_node_t ** s_node);

int dijkstra_pathfind();

int search_graph_ini(path_req_t * p, graph_t * g, s_graph_t * s_graph);
int search_graph_end(s_graph_t * s_graph);

int path_req_ini(path_req_t * p, uint64_t start_id, uint64_t end_id);
int path_req_end(path_req_t * p);

#endif
