#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>

#include "vector.h"
#include "graph_node.h"
#include "graph.h"
#include "traverse.h"
#include "debug.h"
#include "error.h"

/*
 *	This is a temporary, indev debug main file.
 */

int main() {
	
	printf("\n* ------------START TEST 1----------- *\n\n");
	test_end_not_connected();
	printf("\n* ------------START TEST 2----------- *\n\n");
	test_normal_1();
	printf("\n* ------------START TEST 3----------- *\n\n");
	test_normal_2();
	printf("\n* ------------END TESTING------------ *\n\n");

	return 0;
}
