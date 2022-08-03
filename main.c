#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>

#include "vector.h"
#include "graph_node.h"
#include "graph.h"
#include "debug.h"
#include "error.h"

/*
 *	This is a temporary, indev debug main file.
 */

int main() {

	int ret;
	graph_t g;
	uint16_t dat;
	size_t dat_size = sizeof(uint16_t);
	link_t link;
	int64_t w;
	graph_node_t * node;
	graph_node_t * nbr_node;


	ret = graph_ini(&g);
	printf("graph init: %d\n", ret);

	/*
	 *	The following creates an example graph of 6 nodes, laid out in the
	 *  following configuration. Each node stores the letter (e.g.: 'A') as
	 *  its data. The number inside the brackets is that node's ID. The numbers
	 *  on the edges represent the weight.
	 *
	 *  X 3=4 Y:  		X to Y is weight 3, Y to X is weight 4
	 *
	 *  X -5- Y:  		X to Y and Y to X both weight 5
	 *
	 *
	 *
	 *  Configuration of the example graph:
	 *
	 *  A(0) -5- B(1) 3=7 D(3)
	 *  
	 *   |        |        |
	 *   3        2        5
	 *   |        |        |
	 *
	 *  C(2) 6=4 E(4) -1- F(5)
	 *
	 */

	//Creating nodes for graph
	dat = 65;
	ret = graph_add(&g, (char *) &dat, dat_size);
	dat = 66;
	ret = graph_add(&g, (char *) &dat, dat_size);
	dat = 67;
	ret = graph_add(&g, (char *) &dat, dat_size);
	dat = 68;
	ret = graph_add(&g, (char *) &dat, dat_size);
	dat = 69;
	ret = graph_add(&g, (char *) &dat, dat_size);
	dat = 70;
	ret = graph_add(&g, (char *) &dat, dat_size);


	//Linking nodes
	build_link(&link, 0, 1, 5, 5);
	ret = graph_link_nodes(&g, link);

	build_link(&link, 0, 2, 3, 3);
	ret = graph_link_nodes(&g, link);

	build_link(&link, 1, 3, 3, 7);
	ret = graph_link_nodes(&g, link);
	
	build_link(&link, 1, 4, 2, 2);
	ret = graph_link_nodes(&g, link);
	
	build_link(&link, 2, 4, 6, 4);
	ret = graph_link_nodes(&g, link);

	build_link(&link, 3, 5, 5, 5);
	ret = graph_link_nodes(&g, link);

	build_link(&link, 4, 5, 1, 1);
	ret = graph_link_nodes(&g, link);


	//Checking node links
	ret = get_graph_node_by_id(&g, 0, &node);
	ret = graph_node_get_nbr(node, 0, &nbr_node, &w);
	printf("Neighbour of node 0: id = %lu\n", nbr_node->id);
	ret = graph_node_get_nbr(node, 1, &nbr_node, &w);
	printf("Neighbour of node 0: id = %lu\n", nbr_node->id);
	printf("\n");

	ret = get_graph_node_by_id(&g, 1, &node);
	ret = graph_node_get_nbr(node, 0, &nbr_node, &w);
	printf("Neighbour of node 1: id = %lu\n", nbr_node->id);
	ret = graph_node_get_nbr(node, 1, &nbr_node, &w);
	printf("Neighbour of node 1: id = %lu\n", nbr_node->id);
	ret = graph_node_get_nbr(node, 2, &nbr_node, &w);
	printf("Neighbour of node 1: id = %lu\n", nbr_node->id);
	printf("\n");

	ret = get_graph_node_by_id(&g, 2, &node);
	ret = graph_node_get_nbr(node, 0, &nbr_node, &w);
	printf("Neighbour of node 2: id = %lu\n", nbr_node->id);
	ret = graph_node_get_nbr(node, 1, &nbr_node, &w);
	printf("Neighbour of node 2: id = %lu\n", nbr_node->id);
	printf("\n");

	ret = get_graph_node_by_id(&g, 3, &node);
	ret = graph_node_get_nbr(node, 0, &nbr_node, &w);
	printf("Neighbour of node 3: id = %lu\n", nbr_node->id);
	ret = graph_node_get_nbr(node, 1, &nbr_node, &w);
	printf("Neighbour of node 3: id = %lu\n", nbr_node->id);
	printf("\n");

	ret = get_graph_node_by_id(&g, 4, &node);
	ret = graph_node_get_nbr(node, 0, &nbr_node, &w);
	printf("Neighbour of node 4: id = %lu\n", nbr_node->id);
	ret = graph_node_get_nbr(node, 1, &nbr_node, &w);
	printf("Neighbour of node 4: id = %lu\n", nbr_node->id);
	ret = graph_node_get_nbr(node, 2, &nbr_node, &w);
	printf("Neighbour of node 4: id = %lu\n", nbr_node->id);
	printf("\n");

	ret = get_graph_node_by_id(&g, 5, &node);
	ret = graph_node_get_nbr(node, 0, &nbr_node, &w);
	printf("Neighbour of node 5: id = %lu\n", nbr_node->id);
	ret = graph_node_get_nbr(node, 1, &nbr_node, &w);
	printf("Neighbour of node 5: id = %lu\n", nbr_node->id);
	printf("\n");



	
	//Check (in GDB) all of A's neighbours before removing B
	ret = get_graph_node_by_id(&g, 0, &node);
	ret = graph_node_get_nbr(node, 0, &nbr_node, &w);
	ret = graph_node_get_nbr(node, 1, &nbr_node, &w);

	//Check (in GDB) all of E's neighbours before removing B
	ret = get_graph_node_by_id(&g, 4, &node);
	ret = graph_node_get_nbr(node, 0, &nbr_node, &w);
	ret = graph_node_get_nbr(node, 1, &nbr_node, &w);
	ret = graph_node_get_nbr(node, 2, &nbr_node, &w);
	
	//Remove B from graph
	ret = graph_rmv(&g, 1);
	printf("Remove of ID 1: %d\n", ret);

	//Check (in GDB) all of A's neighbours after removing B
	ret = get_graph_node_by_id(&g, 0, &node);
	ret = graph_node_get_nbr(node, 0, &nbr_node, &w);

	//Check (in GDB) all of E's neighbours after removing B
	ret = get_graph_node_by_id(&g, 4, &node);
	ret = graph_node_get_nbr(node, 0, &nbr_node, &w);
	ret = graph_node_get_nbr(node, 1, &nbr_node, &w);

	return 0;
}
