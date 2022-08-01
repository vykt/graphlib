#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>

#include "vector.h"
#include "graph_node.h"
#include "graph.h"
#include "error.h"

void end() {
	printf("uwu\n");
}

int main() {

	int ret;
	graph_t g;

	//graph_node_t * node = malloc(sizeof(graph_node_t));

	graph_node_t * node;

	uint16_t dat = 75;

	ret = graph_ini(&g);
	
	ret = graph_add(&g, (char *) &dat, sizeof(uint16_t));
	printf("add ret 1: %d\n", ret);
	printf("VALUE! %u\n", (uint16_t) *(((graph_node_t *) g.nodes.vector)->data));

	dat = 69;
	ret = graph_add(&g, (char *) &dat, sizeof(uint16_t));
	printf("add ret 2: %d\n", ret);
	uint64_t index = 1;
	printf("VALUE! %u\n", (uint16_t) *(((graph_node_t *) g.nodes.vector + (g.nodes.data_size * index))->data));

	dat = 17;
	ret = graph_add(&g, (char *) &dat, sizeof(uint16_t));
	printf("add ret 3: %d\n", ret);
	index = 1;
	printf("VALUE! %u\n", (uint16_t) *(((graph_node_t *) g.nodes.vector + (g.nodes.data_size * index))->data));

	uint16_t * dat2;
	ret = graph_get_dat(&g, 0, (char **) &dat2);
	//printf("VALUE 1? %u\n", *dat2);

	ret = graph_get_dat(&g, 1, (char **) &dat2);
	//printf("VALUE 2? %u\n", *dat2);

	ret = graph_get_dat(&g, 2, (char **) &dat2);
	//printf("VALUE 3? %u\n", *dat2);

	end();

	return 0;
}
