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


int main() {

	int ret;
	graph_t g;
	uint16_t dat;
	size_t dat_size = sizeof(uint16_t);
	graph_node_t * n;

	ret = graph_ini(&g);
	printf("graph init: %d\n", ret);

	dat = 75;
	ret = graph_add(&g, (char *) &dat, dat_size);
	printf("graph add: %d\n", ret);

	dat = 76;
	ret = graph_add(&g, (char *) &dat, dat_size);
	printf("graph add: %d\n", ret);

	dat = 77;
	ret = graph_add(&g, (char *) &dat, dat_size);
	printf("graph add: %d\n", ret);


	//CHANGING DATA VALUES
	/*dat = 80;
	uint64_t dat2 = 888888;
	size_t dat2_size = sizeof(uint64_t);
	ret = graph_set_data(&g, 1, (char *) &dat2, dat2_size);

	uint64_t * dat2_get = malloc(dat2_size);
	ret = graph_get_data(&g, 1, (char **) &dat2_get);

	printf("dat_get: %lu\n", *dat2_get);

	size_t size;
	size = graph_get_data_size(&g, 1);
	printf("size of data: %lu\n", size);
	*/

	//LINKING NODES
	link_t link;
	build_link(&link, 0, 1, 5, 8);

	printf("id1: %lu, id2: %lu, w1: %ld, w2: %ld\n",
		   link.id, link.id_target, link.weight_to, link.weight_from);

	ret = graph_link_nodes(&g, link);
	printf("link ret: %d\n", ret);

	build_link(&link, 0, 2, 6, 6);
	ret = graph_link_nodes(&g, link);
	
	//WORK PLS!
	/*ret = get_graph_node_by_id(&g, 0, &n);
	graph_node_t * nbr_node;
	int64_t w = 0;

	ret = graph_node_get_nbr(n, 0, &nbr_node, &w);
	printf("Neighbour of node 0: id = %lu\n", nbr_node->id);
	
	ret = get_graph_node_by_id(&g, 0, &n);
	ret = graph_node_get_nbr(n, 1, &nbr_node, &w);
	printf("Neighbour of node 0: id = %lu\n", nbr_node->id);

	ret = get_graph_node_by_id(&g, 1, &n);
	ret = graph_node_get_nbr(n, 0, &nbr_node, &w);
	printf("Neighbour of node 1: id = %lu\n", nbr_node->id);
	*/

	build_link(&link, 0, 1, 0, 0);
	ret = graph_unlink_nodes(&g, link);
	printf("Remove ret: %d\n", ret);

	return 0;
}
