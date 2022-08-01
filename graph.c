#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>

#include "vector.h"
#include "graph_node.h"
#include "graph.h"
#include "error.h"


//TODO debug includes
#include <stdio.h>


int graph_add(graph_t * g, char * data, size_t data_size) {

	/*
	 *	This function is very confusing so read this.
	 *
	 *	g->node.vector = Memory where all nodes live. It is dynamically allocated.
	 *                   Therefore to add a new node, first it has to be expanded
	 *                   to fit it in. This is why first, vector_add is called.
	 *
	 *  Once it is called, there is now all this space in memory but its empty. Now,
	 *  the pointer to this memory can be passed to graph_node_ini to turn that
	 *  memory into a graph_node. However, there are many nodes inside, how to get
	 *  the correct pointer to this new memory? After all its not returned...
	 *
	 *	g->nodes.vector = All nodes. This is a POINTER.
	 *	g->nodes.length = How many nodes there are. Minus 1 and its the index.
	 *	                  The new node is always added to the end of the memory block.
	 *	                  
	 *  g->nodes.vector + (g->nodes.data_size * (g->nodes.length - 1))
	 *
	 *                        aka
	 *
	 *  Base pointer + Size of graph_node * Index of newest node (blank memory atm).
	 */

	//Check for NULL
	if (g == NULL || data == NULL || data_size == 0) return NULL_ERR;

	int ret;

	//Add entry for node
	ret = vector_add(&g->nodes, 0, NULL, VECTOR_APPEND_TRUE);
	if (ret != SUCCESS) return ret;

	//g->nodes.vector[-1] (latest entry)
	graph_node_t * init_p = 
		(graph_node_t *) g->nodes.vector+(g->nodes.data_size * (g->nodes.length-1));
	ret = graph_node_ini(init_p, data, data_size, g->next_id);
	if (ret != SUCCESS) return ret;

	//Update graph tracking values
	g->next_id = g->next_id + 1;
	
	return SUCCESS;
}


int graph_rmv() {

	return SUCCESS;
}


int graph_get_dat(graph_t * g, uint64_t index, char ** dat) {

	//Check for NULL
	if (g == NULL) return NULL_ERR;

	//Check index is in range
	if (index >= g->nodes.length) return OUT_OF_BOUNDS_ERR;

	*dat = ((graph_node_t *) g->nodes.vector + (g->nodes.data_size * index))->data;

	printf("val: %u\n", (uint16_t) *(((graph_node_t *) g->nodes.vector + (g->nodes.data_size * index))->data));

	//printf("uwuuuuu %u\n", (uint16_t) **dat);

	return SUCCESS;
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

	return SUCCESS;
}
