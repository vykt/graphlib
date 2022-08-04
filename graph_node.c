#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "graph_node.h"
#include "error.h"

//TODO debug includes
#include <stdio.h>


int graph_node_set_dat(graph_node_t * n, char * data, size_t data_size) {

	//Check NULL
	if (n == NULL || data == NULL || data_size == 0) return NULL_ERR;

	n->data = realloc(n->data, data_size);
	if (n->data == NULL) return MEM_ERR;
	n->data_size = data_size;

	memcpy(n->data, data, n->data_size);

	return SUCCESS;
}


int graph_node_get_dat(graph_node_t * n, char ** data) {

	//Check NULL
	if (n == NULL || data == NULL) return NULL_ERR;

	memcpy(*data, n->data, n->data_size);
	return SUCCESS;
}


int graph_node_add_nbr(graph_node_t * n, graph_node_t * n_add, int64_t weight) {

	//Check for NULL
	if (n == NULL || n_add == NULL) return NULL_ERR;

	int ret;

	//Write to vectors
	ret = vector_add(&n->neighbours, 0, (char *) &n_add, VECTOR_APPEND_TRUE);
	if (ret != SUCCESS) return ret;

	ret = vector_add(&n->edge_weights, 0, NULL, VECTOR_APPEND_TRUE);
	if (ret != SUCCESS) return ret;

	return SUCCESS;
}


//Remove neighbour by id id_tgt from node pointed to by n
int graph_node_rmv_nbr(graph_node_t * n, uint64_t id_tgt) {
	
	//Check for NULL
	if (n == NULL) return NULL_ERR;
	
	int ret;
	graph_node_t ** nbr = malloc(sizeof(graph_node_t *));
	int64_t * weight = malloc(sizeof(int64_t));

	//For every neighbour
	for (uint64_t i = 0; i < n->neighbours.length; i++) {

		ret = vector_get(&n->neighbours, i, (char *) nbr);
		if (ret != SUCCESS) {
			free(nbr);
			free(weight);
			return ret;
		}

		//If ID matches with target
		if ((*nbr)->id == id_tgt) {
			ret = vector_rmv(&n->neighbours, i);
			if (ret != SUCCESS) {
				free(nbr);
				free(weight);
				return ret;
			}
			ret = vector_rmv(&n->edge_weights, i);	
			if (ret != SUCCESS) {
				free(nbr);
				free(weight);
				return ret;
			}
			free(nbr);
			free(weight);
			return SUCCESS;
		}
	} //End for every vector

	free(nbr);
	free(weight);
	return FAIL;
}


int graph_node_get_nbr(graph_node_t * n, uint64_t index, graph_node_t ** nbr, int64_t * weight) {

	//Check for NULL
	if (n == NULL) return NULL_ERR;

	int ret;

	graph_node_t ** temp_node = malloc(sizeof(graph_node_t *));
	int64_t * temp_weight = malloc(sizeof(int64_t));
	
	ret = vector_get(&n->neighbours, index, (char *) temp_node);
	if (ret != SUCCESS) return ret;

	ret = vector_get(&n->edge_weights, index, (char *) temp_weight);
	if (ret != SUCCESS) return ret;

	//Copy data over so the caller can use it.
	*nbr = *temp_node;
	*weight = *temp_weight;
	
	//Free read buffers and return
	free(temp_node);
	free(temp_weight);
	return SUCCESS;
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
