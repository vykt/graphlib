#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>

#include "vector.h"
#include "graph_node.h"
#include "graph.h"
#include "traverse.h"
#include "error.h"

/*
 *	TESTS FILE.
 *
 *	Each function sets up an example graph and pathfinds between two predetermined
 *	nodes on that graph. Tests are verbose and check returns on purpose. This can
 *	be written much more concisely.
 */


//No path from start to end
void test_end_not_connected() {

	int ret;
	graph_t g;
	
	link_t l;
	uint8_t dat;
	size_t dat_size = sizeof(dat);

	path_req_t p;
	s_graph_t s_graph;
	uint64_t * path_node_id;

	//Init graph
	ret = graph_ini(&g);
	printf("Graph init: %d\n", ret);

	//Creating nodes for graph
	dat = 1;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 0
	printf("Node %u init: %d\n", dat, ret);
	dat = 2;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 1
	printf("Node %u init: %d\n", dat, ret);
	dat = 3;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 2
	printf("Node %u init: %d\n", dat, ret);
	dat = 4;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 3
	printf("Node %u init: %d\n", dat, ret);
	dat = 5;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 4
	printf("Node %u init: %d\n", dat, ret);

	//Linking nodes
	build_link(&l, 0, 1, 5, 5);
	ret = graph_link_nodes(&g, l);
	printf("Link 1 - 2 init: %d\n", ret);
	
	build_link(&l, 1, 2, 6, 6);
	ret = graph_link_nodes(&g, l);
	printf("Link 2 - 3 init: %d\n", ret);
	
	build_link(&l, 0, 2, 15, 15);
	ret = graph_link_nodes(&g, l);
	printf("Link 1 - 3 init: %d\n", ret);
	
	build_link(&l, 2, 3, 2, 2);
	ret = graph_link_nodes(&g, l);
	printf("Link 3 - 4 init: %d\n", ret);

	//Pathfinding
	ret = path_req_ini(&p, 0, 4);
	printf("Path req init: %d\n", ret);

	ret = search_graph_ini(&p, &g, &s_graph);
	printf("Search graph init: %d\n", ret);

	ret = dijkstra_pathfind(&p, &s_graph);
	printf("Dijkstra pathfind: %d\n", ret);

	//Conclude results
	if (ret == FAIL) {
		printf("\nNo path from start to end found.\n");
	} else if (ret != SUCCESS) {
		printf("\nAnother error has occured.\n");
	} else {
		printf("\n---   RESULTS   ---\n");
		printf("Path found. Total cost: %ld\n", p.total_cost);
		printf("Path from node 1 to node 5\n");
		for (uint64_t i = 0; i < p.nodes_stack.length; i++) {
			ret = vector_get_ref(&p.nodes_stack, i, (char **) &path_node_id);
			printf("path node: %lu, id: %lu\n", i, *path_node_id);
		}
		printf("--- RESULTS END ---\n\n");
	}

	//End allocated memory
	ret = search_graph_end(&s_graph);
	printf("Search graph end: %d\n", ret);
	ret = path_req_end(&p);
	printf("Path request end: %d\n", ret);
	ret = graph_end(&g);
	printf("Graph end: %d\n", ret);
}


//Arbitrary graph 1
void test_normal_1() {

	int ret;
	graph_t g;
	
	link_t l;
	uint8_t dat;
	size_t dat_size = sizeof(dat);

	path_req_t p;
	s_graph_t s_graph;
	uint64_t * path_node_id;

	//Init graph
	ret = graph_ini(&g);
	printf("Graph init: %d\n", ret);

	//Creating nodes for graph
	dat = 0;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 0
	printf("Node %u init: %d\n", dat, ret);
	dat = 1;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 1
	printf("Node %u init: %d\n", dat, ret);
	dat = 2;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 2
	printf("Node %u init: %d\n", dat, ret);
	dat = 3;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 3
	printf("Node %u init: %d\n", dat, ret);
	dat = 4;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 4
	printf("Node %u init: %d\n", dat, ret);
	dat = 5;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 5
	printf("Node %u init: %d\n", dat, ret);
	dat = 6;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 6
	printf("Node %u init: %d\n", dat, ret);

	//Linking nodes
	build_link(&l, 0, 1, 2, 2);
	ret = graph_link_nodes(&g, l);
	printf("Link 0 - 1 init: %d\n", ret);
	
	build_link(&l, 0, 2, 6, 6);
	ret = graph_link_nodes(&g, l);
	printf("Link 0 - 2 init: %d\n", ret);
	
	build_link(&l, 1, 3, 5, 5);
	ret = graph_link_nodes(&g, l);
	printf("Link 1 - 3 init: %d\n", ret);
	
	build_link(&l, 2, 3, 8, 8);
	ret = graph_link_nodes(&g, l);
	printf("Link 2 - 3 init: %d\n", ret);
	
	build_link(&l, 3, 4, 10, 10);
	ret = graph_link_nodes(&g, l);
	printf("Link 3 - 4 init: %d\n", ret);
	
	build_link(&l, 3, 5, 15, 15);
	ret = graph_link_nodes(&g, l);
	printf("Link 3 - 5 init: %d\n", ret);
	
	build_link(&l, 4, 5, 6, 6);
	ret = graph_link_nodes(&g, l);
	printf("Link 4 - 4 init: %d\n", ret);
	
	build_link(&l, 4, 6, 2, 2);
	ret = graph_link_nodes(&g, l);
	printf("Link 4 - 6 init: %d\n", ret);
	
	build_link(&l, 5, 6, 6, 6);
	ret = graph_link_nodes(&g, l);
	printf("Link 5 - 6 init: %d\n", ret);

	//Pathfinding
	ret = path_req_ini(&p, 0, 6);
	printf("Path req init: %d\n", ret);

	ret = search_graph_ini(&p, &g, &s_graph);
	printf("Search graph init: %d\n", ret);

	ret = dijkstra_pathfind(&p, &s_graph);
	printf("Dijkstra pathfind: %d\n", ret);

	//Conclude results
	if (ret == FAIL) {
		printf("\nNo path from start to end found.\n");
	} else if (ret != SUCCESS) {
		printf("\nAnother error has occured.\n");
	} else {
		printf("\n---   RESULTS   ---\n");
		printf("Path found. Total cost: %ld\n", p.total_cost);
		for (uint64_t i = 0; i < p.nodes_stack.length; i++) {
			ret = vector_get_ref(&p.nodes_stack, i, (char **) &path_node_id);
			printf("path node: %lu, id: %lu\n", i, *path_node_id);
		}
		printf("--- RESULTS END ---\n\n");
	}

	//End allocated memory
	ret = search_graph_end(&s_graph);
	printf("Search graph end: %d\n", ret);
	ret = path_req_end(&p);
	printf("Path request end: %d\n", ret);
	ret = graph_end(&g);
	printf("Graph end: %d\n", ret);
}


//Arbitrary graph 2
void test_normal_2() {

	int ret;
	graph_t g;
	
	link_t l;
	uint8_t dat;
	size_t dat_size = sizeof(dat);

	path_req_t p;
	s_graph_t s_graph;
	uint64_t * path_node_id;

	//Init graph
	ret = graph_ini(&g);
	printf("Graph init: %d\n", ret);

	//Creating nodes for graph
	dat = 0;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 0
	printf("Node %u init: %d\n", dat, ret);
	dat = 1;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 1
	printf("Node %u init: %d\n", dat, ret);
	dat = 2;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 2
	printf("Node %u init: %d\n", dat, ret);
	dat = 3;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 3
	printf("Node %u init: %d\n", dat, ret);
	dat = 4;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 4
	printf("Node %u init: %d\n", dat, ret);
	dat = 5;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 5
	printf("Node %u init: %d\n", dat, ret);	
	dat = 6;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 6
	printf("Node %u init: %d\n", dat, ret);
	dat = 7;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 6
	printf("Node %u init: %d\n", dat, ret);
	dat = 8;
	ret = graph_add(&g, (char *) &dat, dat_size); //ID 6
	printf("Node %u init: %d\n", dat, ret);

	//Linking nodes
	build_link(&l, 0, 1, 4, 4);
	ret = graph_link_nodes(&g, l);
	printf("Link 0 - 1 init: %d\n", ret);
	
	build_link(&l, 0, 7, 8, 8);
	ret = graph_link_nodes(&g, l);
	printf("Link 0 - 2 init: %d\n", ret);
	
	build_link(&l, 1, 2, 8, 8);
	ret = graph_link_nodes(&g, l);
	printf("Link 1 - 3 init: %d\n", ret);
	
	build_link(&l, 1, 7, 11, 11);
	ret = graph_link_nodes(&g, l);
	printf("Link 2 - 3 init: %d\n", ret);
	
	build_link(&l, 7, 8, 7, 7);
	ret = graph_link_nodes(&g, l);
	printf("Link 3 - 4 init: %d\n", ret);
	
	build_link(&l, 7, 6, 1, 1);
	ret = graph_link_nodes(&g, l);
	printf("Link 3 - 5 init: %d\n", ret);
	
	build_link(&l, 2, 8, 2, 2);
	ret = graph_link_nodes(&g, l);
	printf("Link 4 - 4 init: %d\n", ret);
	
	build_link(&l, 8, 6, 6, 6);
	ret = graph_link_nodes(&g, l);
	printf("Link 4 - 6 init: %d\n", ret);
	
	build_link(&l, 2, 3, 7, 7);
	ret = graph_link_nodes(&g, l);
	printf("Link 5 - 6 init: %d\n", ret);

	build_link(&l, 2, 5, 4, 4);
	ret = graph_link_nodes(&g, l);
	printf("Link 3 - 4 init: %d\n", ret);
	
	build_link(&l, 3, 5, 14, 14);
	ret = graph_link_nodes(&g, l);
	printf("Link 3 - 5 init: %d\n", ret);
	
	build_link(&l, 3, 4, 9, 9);
	ret = graph_link_nodes(&g, l);
	printf("Link 4 - 4 init: %d\n", ret);
	
	build_link(&l, 5, 4, 10, 10);
	ret = graph_link_nodes(&g, l);
	printf("Link 4 - 6 init: %d\n", ret);
	
	build_link(&l, 6, 5, 2, 2);
	ret = graph_link_nodes(&g, l);
	printf("Link 5 - 6 init: %d\n", ret);
	
	//Pathfinding
	ret = path_req_ini(&p, 0, 4);
	printf("Path req init: %d\n", ret);

	ret = search_graph_ini(&p, &g, &s_graph);
	printf("Search graph init: %d\n", ret);

	ret = dijkstra_pathfind(&p, &s_graph);
	printf("Dijkstra pathfind: %d\n", ret);

	//Conclude results
	if (ret == FAIL) {
		printf("\nNo path from start to end found.\n");
	} else if (ret != SUCCESS) {
		printf("\nAnother error has occured.\n");
	} else {
		printf("\n---   RESULTS   ---\n");
		printf("Path found. Total cost: %ld\n", p.total_cost);
		for (uint64_t i = 0; i < p.nodes_stack.length; i++) {
			ret = vector_get_ref(&p.nodes_stack, i, (char **) &path_node_id);
			printf("path node: %lu, id: %lu\n", i, *path_node_id);
		}
		printf("--- RESULTS END ---\n\n");
	}

	//End allocated memory
	ret = search_graph_end(&s_graph);
	printf("Search graph end: %d\n", ret);
	ret = path_req_end(&p);
	printf("Path request end: %d\n", ret);
	ret = graph_end(&g);
	printf("Graph end: %d\n", ret);
}
