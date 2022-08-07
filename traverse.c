#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>

#include "vector.h"
#include "graph_node.h"
#include "graph.h"
#include "traverse.h"
#include "error.h"


/*
 *	Has to be performed atomically without modifying the graph. Otherwise
 *	result is undefined.
 */


//Pathfind using dijkstra's, non recursively this time
int dijkstra_pathfind(path_req_t * p, s_graph_t * s_graph) {

	int ret;
	s_node_t * s_node;

	//For every node on the graph
	for (uint64_t i = 0; i < s_graph->graph->nodes.length; i++) {

		ret = 0; //TODO finish

	} //End for every node

	return SUCCESS;
}


//Wrapper for graph node, adding search info
int s_node_ini(graph_t * g, s_node_t * s_node, graph_node_t * node) {

	//Set values
	s_node->node = node;
	s_node->prev_search_node = NULL;
	s_node->cost = 0;
	s_node->visited = 0;

	return SUCCESS;
}


/*
 *  Wrap every node in graph with search metadata. Push all nodes on queue,
 *  move starting node to beginning of queue.
 */

//Initialise graph search structure
int search_graph_ini(path_req_t * p, graph_t * g, s_graph_t * s_graph) {

	int ret;
	graph_node_t * temp_node;
	s_node_t * s_mem = malloc(sizeof(s_node_t));
	if (s_mem == NULL) return MEM_ERR;

	s_graph->graph = g;

	ret = vector_ini(&s_graph->s_queue, sizeof(s_node_t));
	if (ret != SUCCESS) return ret;

	//For every graph node, add an entry to queue and build it
	for (uint64_t i = 0; i < g->nodes.length; i++) {

		//Expand vector
		ret = vector_add(&s_graph->s_queue, 0, NULL, VECTOR_APPEND_TRUE);
		if (ret != SUCCESS) { free(s_mem); return ret; }

		//Create next s_node & initialise it
		ret = vector_get_ref(&g->nodes, i, (char **) &temp_node);
		if (ret != SUCCESS) { free(s_mem); return ret; }
		ret = s_node_ini(g, s_mem, temp_node);
		if (ret != SUCCESS) { free(s_mem); return ret; }

		//Add new s_node to search queue (for now, default order)
		ret = vector_set(&s_graph->s_queue, i, (char *) s_mem);

		//If ID matches start, move to index 0
		if (s_mem->node->id == p->start_id) {
			ret = vector_mov(&s_graph->s_queue, i, 0);
			if (ret != SUCCESS) { free(s_mem); return ret; }
		}

	} //End for every graph node

	return SUCCESS;
}


//End graph search structure
int search_graph_end(s_graph_t * s_graph) {

	int ret;
	ret = vector_end(&s_graph->s_queue);
	if (ret != SUCCESS) return ret;

	return SUCCESS;
}


//Initialise path request structure
int path_req_ini(path_req_t * p, uint64_t start_id, uint64_t end_id) {

	int ret;
	p->start_id = start_id;
	p->end_id = end_id;
	ret = vector_ini(&p->nodes_stack, sizeof(uint64_t));
	if (ret != SUCCESS) return ret;

	return SUCCESS;
};


//End path request structure
int path_req_end(path_req_t * p) {
	
	int ret;
	ret = vector_end(&p->nodes_stack);
	if (ret != SUCCESS) return ret;

	return SUCCESS;
};
