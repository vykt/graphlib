#ifndef GRAPH_H
#define GRAPH_H

#include <sys/types.h>
#include <stdint.h>

#include "vector.h"
#include "graph_node.h"


typedef struct graph graph_t;
typedef struct link_data link_t;

struct graph {

	//When calling vector_get() on graph, return is a graph_node_t
	vector_t nodes;   //stores graph_node_t *
	uint64_t next_id; //ID to assign to next new node

};

struct link_data {

	uint64_t id;         //from this node, node 1
	uint64_t id_target;  //to this node, node 2
	int64_t weight_to;   //weight of edge from node 1 to node 2
	int64_t weight_from; //weight of edge from node 2 to node 1

};


int get_graph_node_by_id(graph_t * g, uint64_t id, graph_node_t ** node);
int get_graph_node_index_by_id(graph_t * g, uint64_t id, uint64_t * index);

int graph_add(graph_t * g, char * data, size_t data_size); //Add node
int graph_rmv(graph_t * g, uint64_t id);

void build_link(link_t * link, uint64_t id, uint64_t id_tgt, int64_t w_to, int64_t w_fr);
int graph_link_nodes(graph_t * g, link_t link);
int graph_unlink_nodes(graph_t * g, link_t link);

int graph_set_data(graph_t * g, uint64_t id, char * data, size_t data_size);
int graph_get_data(graph_t * g, uint64_t id, char ** data);
size_t graph_get_data_size(graph_t * g, uint64_t id);

int graph_ini(graph_t * g);
int graph_end(graph_t * g);


#endif
