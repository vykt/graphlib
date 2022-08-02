#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

#include "vector.h"
#include "graph_node.h"
#include "graph.h"
#include "error.h"


//TODO debug includes
#include <stdio.h>


//Get graph node pointer by id
int get_graph_node_by_id(graph_t * g, uint64_t id, graph_node_t ** node) {

	int ret;
	graph_node_t * temp_node;
	
	//For every node
	for (uint64_t i = 0; i < g->nodes.length; i++) {

		ret = vector_get_ref(&g->nodes, i, (char **) &temp_node);
		if (ret != SUCCESS) return ret;

		//If matched
		if (temp_node->id == id) {
			*node = temp_node;
			return SUCCESS;
		}
	} //End for every node
	return FAIL;
}


//Add new node to graph
int graph_add(graph_t * g, char * data, size_t data_size) {

	//Check for NULL
	if (g == NULL || data == NULL || data_size == 0) return NULL_ERR;

	int ret;

	ret = vector_add(&g->nodes, 0, NULL, VECTOR_APPEND_TRUE);
	if (ret != SUCCESS) return ret;

	graph_node_t * new_node;
	ret = vector_get_ref(&g->nodes, g->nodes.length - 1, (char **) &new_node);
	if (ret != SUCCESS) return ret;

	ret = graph_node_ini(new_node, data, data_size, g->next_id);
	if (ret != SUCCESS) return ret;

	g->next_id = g->next_id + 1;

	return SUCCESS;

}


//Remove node from graph, by ID
//TODO: Also remove edge connections of neighours
int graph_rmv(graph_t * g, uint64_t index) {

	return SUCCESS;
}


int graph_set_data(graph_t * g, uint64_t id, char * data, size_t data_size) {

	int ret;
	graph_node_t * node;

	ret = get_graph_node_by_id(g, id, &node);
	if (ret != SUCCESS) return ret;

	ret = graph_node_set_dat(node, data, data_size);
	if (ret != SUCCESS) return ret;

	return SUCCESS;

}


int graph_get_data(graph_t * g, uint64_t id, char ** data) {

	//Check for NULL
	if (g == NULL) return NULL_ERR;

	//Check index is in range
	if (id >= g->nodes.length) return OUT_OF_BOUNDS_ERR;

	int ret;
	graph_node_t * node;

	ret = get_graph_node_by_id(g, id, &node);
	if (ret != SUCCESS) return ret;

	ret = graph_node_get_dat(node, data);

	//memcpy(*data, node->data, node->data_size);

	return SUCCESS;
}


size_t graph_get_data_size(graph_t * g, uint64_t id) {

	int ret;
	graph_node_t * node;

	ret = get_graph_node_by_id(g, id, &node);
	if (ret != SUCCESS) return ret;

	return node->data_size;
}


int graph_ini(graph_t * g) {

	//Check for NULL
	if (g == NULL) return NULL_ERR;

	//Set default values & init vector
	int ret;
	ret = vector_ini(&g->nodes, sizeof(graph_node_t));
	if (ret != SUCCESS) return ret;
	
	g->next_id = 0;

	return SUCCESS;
}


int graph_end(graph_t * g) {

	int ret;
	ret = vector_end(&g->nodes);
	if (ret != SUCCESS) return ret;

	return SUCCESS;
}
