#ifndef VECTOR_H
#define VECTOR_H

#include <sys/types.h>
#include <stdint.h>

#define VECTOR_SUCCESS 0
#define VECTOR_FULL_ERR 1
#define VECTOR_EMPTY_ERR 1
#define VECTOR_OUT_OF_BOUNDS_ERR 1
#define VECTOR_MEM_ERR 2
#define VECTOR_NULL_ERR 3

#define VECTOR_APPEND_TRUE 1
#define VECTOR_APPEND_FALSE 0

typedef struct vector vector_t;

struct vector {

	char * vector;
	size_t data_size;
	uint64_t length;

};


int vector_add(vector_t * v, uint64_t pos, char * data, uint8_t append);
int vector_rmv(vector_t * v, uint64_t pos);
int vector_get(vector_t * v, uint64_t pos, char * read_data);

int vector_ini(vector_t * v, size_t data_size);
int vector_end(vector_t * v);

#endif
