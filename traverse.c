#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <limits.h>

#include "vector.h"
#include "graph_node.h"
#include "graph.h"
#include "traverse.h"
#include "error.h"


/*
 *	Has to be performed atomically without modifying the graph. Otherwise
 *	result is undefined.
 */


int queue_smart_move(vector_t * s_queue, s_node_t * s_node) {

	int ret;
	s_node_t * temp_node;
	uint64_t s_node_index;

	//For each node in queue, minus limit
	for (uint64_t i = 0; i < s_queue->length; i++) {

		ret = vector_get_ref(s_queue, i, (char **) &temp_node);
		if (ret != SUCCESS) return ret;

		//If s_node should be moved to this position
		if (s_node->cost < temp_node->cost) {

			//Get index of s_node
			ret = get_index_by_s_node(s_queue, s_node, &s_node_index);
			if (ret != SUCCESS) return ret;

			//Move search node to ordered place in queue
			ret = vector_mov(s_queue, s_node_index, i); //No op on s_node_index == i
			if (ret != SUCCESS) return ret;
		} //End if s_node should be moved to this position

	} //End for each node

	return SUCCESS;
}


int get_index_by_s_node(vector_t * s_queue, s_node_t * s_node, uint64_t * index) {

	int ret;
	s_node_t * temp_s_node;

	//For every search node
	for (uint64_t i = 0; i < s_queue->length; i++) {

		ret = vector_get_ref(s_queue, i, (char **) &temp_s_node);
		if (ret != SUCCESS) return ret;

		if (temp_s_node == s_node) {
			*index = i;
			return SUCCESS;
		}
	}// End for every search node
	
	return FAIL;
}


int get_s_node_by_id(s_graph_t * s_graph, uint64_t id, s_node_t ** s_node) {

	int ret;
	graph_node_t * temp_node;
	ret = get_graph_node_by_id(s_graph->graph, id, (graph_node_t **) &temp_node);

	//For each node
	for (uint64_t i = 0; i < s_graph->graph->nodes.length; i++) {

		ret = vector_get_ref(&s_graph->s_queue, i, (char **) s_node);
		if (ret != SUCCESS) return ret;

		if ((*s_node)->node == temp_node) return SUCCESS;
	} //End for each node
	return FAIL;
}


//Pathfind using dijkstra's, non recursively this time
int dijkstra_pathfind(path_req_t * p, s_graph_t * s_graph) {

	int ret;
	s_node_t * s_node;
	
	uint64_t * nbr_id;
	int64_t * nbr_weight;
	s_node_t * nbr_s_node;
	
	int64_t cur_weight = 0;

	//For every node on the graph
	for (uint64_t i = 0; i < s_graph->graph->nodes.length; i++) {

		ret = vector_get_ref(&s_graph->s_queue, 0, (char **) &s_node);
		if (ret != SUCCESS) return ret;

		//On first run, set cost of starting node to 0
		if (i == 0) {
			s_node->cost = 0;
		}

		//For every neighbour of node
		for (uint64_t j = 0; j < s_node->node->neighbours.length; j++) {
			
			//Get id & weight of neighbour indexed at j
			ret = vector_get_ref(&s_node->node->neighbours, j, (char **) &nbr_id);
			if (ret != SUCCESS) return ret;
			ret = vector_get_ref(&s_node->node->edge_weights, j, (char **) &nbr_weight);
			if (ret != SUCCESS) return ret;

			//Get neighbour's s_node
			ret = get_s_node_by_id(s_graph, *nbr_id, &nbr_s_node);
			if (ret != SUCCESS) return ret;

			//Set nbr s_node's cost, prev search_node
			nbr_s_node->cost = cur_weight + *nbr_weight;
			nbr_s_node->prev_search_node = s_node;



			/*
			 *  Move neighbour
			 *
			 *	In preparation for next iteration, set cur_weight to cost of
			 *	next iteration
			 */
		}

	} //End for every node

	return SUCCESS;
}


//Wrapper for graph node, adding search info
int s_node_ini(graph_t * g, s_node_t * s_node, graph_node_t * node) {

	//Set values
	s_node->node = node;
	s_node->prev_search_node = NULL;
	s_node->cost = LONG_MAX;
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

		//If ID matches start, move to index 0 and set cost to 0
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
