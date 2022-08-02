#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>

#include "vector.h"
#include "graph_node.h"
#include "graph.h"
#include "error.h"


int main() {

	int ret;
	graph_t g;
	uint16_t dat;
	size_t dat_size = sizeof(uint16_t);

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

	//ret = graph_rmv(&g, 1);

	dat = 80;
	uint64_t dat2 = 888888;
	size_t dat2_size = sizeof(uint64_t);
	ret = graph_set_data(&g, 1, (char *) &dat2, dat2_size);

	uint64_t * dat2_get = malloc(dat2_size);
	ret = graph_get_data(&g, 1, (char **) &dat2_get);

	printf("dat_get: %lu\n", *dat2_get);
	
	return 0;
}
