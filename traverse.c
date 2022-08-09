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


int get_cur_weight(s_node_t * s_node, s_node_t * s_start_node, int64_t * cur_weight) {

	int ret;
	int index;
	int64_t * weight;

	//If no more previous nodes
	if (s_node->prev_search_node == NULL) {
		
		//If final matches start node
		if (s_node == s_start_node) {
			return SUCCESS;
		
		//Else final node doesn't match start node
		} else {
			return FAIL;
		}	
		
	//If there are more previous nodes
	} else {
		
		//Get index of prev node
		ret = get_nbr_index_by_id(s_node->prev_search_node, s_node->node->id, &index);
		if (ret != SUCCESS) return ret;

		//Get weight of edge from prev node to this node
		ret = vector_get_ref(&s_node->prev_search_node->node->edge_weights, index,
				             (char **) &weight);

		//Add weight of this edge to the total cost
		*cur_weight = *cur_weight + *weight;

		//Call function again (recurse), now providing previous node in path
		ret = get_cur_weight(s_node->prev_search_node, s_start_node, cur_weight);
		if (ret != SUCCESS) return ret;

	}
	

}


int get_nbr_index_by_id(s_node_t * s_node, uint64_t id, uint64_t * index) {

	int ret;
	uint64_t * nbr_id;

	//For each neighbour
	for (uint64_t i = 0; i < s_node->node->neighbours.length; i++) {
		
		ret = vector_get_ref(&s_node->node->neighbours, i, (char **) &nbr_id);
		if (ret != SUCCESS) return ret;

		if (*nbr_id == id) {
			*index = i;
			return SUCCESS;
		}
	}

	return FAIL;
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
	s_node_t * s_start_node;
	
	uint64_t * nbr_id;
	int64_t * nbr_weight;
	s_node_t * nbr_s_node;
	
	int64_t cur_weight;
	char nodes_stack_incomplete = 1;

	//For every node on the graph
	for (uint64_t i = 0; i < s_graph->graph->nodes.length; i++) {

		//Get node indexed at i in search queue
		ret = vector_get_ref(&s_graph->s_queue, i, (char **) &s_node);
		if (ret != SUCCESS) return ret;

		//On first run, set cost of starting node to 0
		if (i == 0) {
			s_node->cost = 0;
			s_start_node = s_node;
		}

		//Recursively calculate cur_weight for this iteration using prev_node
		cur_weight = 0;
		ret = get_cur_weight(s_node, s_start_node, &cur_weight);
		if (ret != SUCCESS) return ret;

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

			//If this path is shorter
			if (nbr_s_node->cost > cur_weight + *nbr_weight) {
				
				//Set nbr s_node's cost and prev search_node
				nbr_s_node->cost = cur_weight + *nbr_weight;
				nbr_s_node->prev_search_node = s_node;
			
				//Move neighbour
				ret = queue_smart_move(&s_graph->s_queue, s_node);
			}

		} //End for every neighbour of node

		s_node->visited = 1;

	} //End for every node

	//Fetch end node, get ID
	uint64_t * id_write = malloc(sizeof(uint64_t));
	if (id_write == NULL) return MEM_ERR;

	ret = get_s_node_by_id(s_graph, p->end_id, &s_node);
	if (ret != SUCCESS) return ret;

	memcpy(id_write, s_node->node->id);
	
	//Add node to stack, fetch previous node
	while (nodes_stack_imcomplete) {

		//Add id
		ret = vector_add(&p->nodes_stack , 0, (char *) s_mem, VECTOR_APPEND_FALSE);
		if (ret != SUCCESS) return ret;

		//TODO
		//
		//Check if prev ID available, if yes set s_node to it.
		//
		//Find appropriate time to break this loop.
		//
		//Test, test it all!

	}

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
