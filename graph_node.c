#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "graph_node.h"
#include "error.h"

//TODO debug includes
#include <stdio.h>


void graph_node_set_dat(graph_node_t * n, char * data) {

	memcpy(n->data, data, n->data_size);

}


int graph_node_add_nbr(graph_node_t * n, graph_node_t * neighbour, int64_t weight) {

	//Check for NULL
	if (n == NULL || neighbour == NULL) {
		return NULL_ERR;
	}

	//Try add neighbour
	int ret;
	ret = vector_add(&n->neighbours, 0, (char *) neighbour, VECTOR_APPEND_TRUE);
	if (ret != SUCCESS) {
		return ret;
	}
	//Tru add weight
	ret = vector_add(&n->edge_weights, 0, (char *) &weight, VECTOR_APPEND_TRUE);
	if (ret != SUCCESS) {
		return ret;
	}

	return SUCCESS;

}


int graph_node_rmv_nbr(graph_node_t * n, uint64_t id) {

	//Check for NULL
	if (n == NULL) {
		return NULL_ERR;
	}

	//Find neighbour by ID
	int ret;
	graph_node_t graph_node_searched;
	for (int i = 0; i < n->neighbours.length; i++) {

		//Get next neighbour
		ret = vector_get(&n->neighbours, i, (char *) &graph_node_searched);
		if (ret != SUCCESS) {
			return ret;
		}
		//If IDs match
		if (graph_node_searched.id == id) {
			ret = vector_rmv(&n->neighbours, i);
			if (ret != SUCCESS) {
				return GRAPH_NODE_RMV_NEIGHBOUR_ERR;
			}
			ret = vector_rmv(&n->edge_weights, i);
			if (ret != SUCCESS) {
				return GRAPH_NODE_RMV_WEIGHT_ERR;
			}
			return SUCCESS;
		} //End if IDs match

	} //End for
	return FAIL;
}


int graph_node_ini(graph_node_t * n, char * data, size_t data_size, uint64_t id) {

    //Check for NULL
    if (n == NULL || data_size == 0) {
        return NULL_ERR; 
	}      
    
	//Set data values           
    n->data = malloc(data_size);
    if (n->data == NULL) {        
        return MEM_ERR;
    }
	n->data_size = data_size;
	n->visited = GRAPH_NODE_NOT_VISITED;
	n->id = id;

	//Set data
	n->data = malloc(n->data_size);
	if (n->data == NULL) {
		return NULL_ERR;
	}
	memcpy(n->data, data, data_size);

	//Init vectors
	int ret;

	ret = vector_ini(&n->neighbours, sizeof(graph_node_t *));
	if (ret == NULL_ERR) {
		return NULL_ERR;
	}

	ret = vector_ini(&n->edge_weights, sizeof(int64_t));
	if (ret == NULL_ERR) {
		return NULL_ERR;
	}

	//Return
	return SUCCESS;
}


int graph_node_end(graph_node_t * n) {

	//Check for NULL
	if (n == NULL) {
		return NULL_ERR;
	}

	//TODO Remove reference to node in neighbours
	int ret;

	//Free vectors
	ret = vector_end(&n->neighbours);
	if (ret == NULL_ERR) {
		return NULL_ERR;
	}

	ret = vector_end(&n->edge_weights);
	if (ret == NULL_ERR) {
		return NULL_ERR;
	}
	
	//Return
	return SUCCESS;
}
