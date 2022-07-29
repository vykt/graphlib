#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "vector.h"

//TODO DEBUG INCLUDE
#include <stdio.h>
#include <errno.h>


//Add element
int vector_add(vector_t * v, uint64_t pos, char * data, uint8_t append) {

	//Check for NULL
	if (v == NULL || data == NULL || v->data_size == 0) {
		return VECTOR_NULL_ERR;
	}

	//Check if max capacity reached
	if (v->length == sizeof(uint64_t) - 1) {
		return VECTOR_FULL_ERR;
	}

	//Check if asking for data out of bounds
	if (pos > v->length) {
		return VECTOR_OUT_OF_BOUNDS_ERR;
	}

	//Check for append
	if (append == VECTOR_APPEND_TRUE) {
		pos = v->length;
	}

	v->length = v->length + 1;
	v->vector = realloc(v->vector, (size_t) (v->length * v->data_size));
	if (v->vector == NULL) {
		return VECTOR_MEM_ERR;
	}

	//Move vector entries	
	if (pos != v->length - 1) {	
		memmove(
			v->vector + ((pos + 1) * v->data_size),
			v->vector + (pos * v->data_size),
			((v->length - 1 - pos) * v->data_size)
		);
	}

	//Add new entry
	memcpy(v->vector + (pos * v->data_size), data, v->data_size);
	
	return VECTOR_SUCCESS;
}


//Remove element
int vector_rmv(vector_t * v, uint64_t pos) {

	//Check for NULL
	if (v == NULL) {
		return VECTOR_NULL_ERR;
	}

	//Check if vector is empty
	if (v->length == 0) {
		return VECTOR_EMPTY_ERR;
	}

	//Check if asking for data out of bounds
	if (pos >= v->length) {
		return VECTOR_OUT_OF_BOUNDS_ERR;
	}

	if (pos < v->length - 1) {
		memmove(
			v->vector + (pos * v->data_size),
			v->vector + ((pos + 1) * v->data_size),
			(v->length - 1 - pos) * v->data_size
		);
	} //End if

	v->length = v->length - 1;
	v->vector = realloc(v->vector, v->length * v->data_size);
	
	if (v->vector == NULL) {
		return VECTOR_MEM_ERR;
	}

	return VECTOR_SUCCESS;
}


//Get element
int vector_get(vector_t * v, uint64_t pos, char * data) {

	//Check for NULL
	if (v == NULL) {
		return VECTOR_NULL_ERR;
	}

	//Check if vector is empty
	if (v->length == 0) {
		return VECTOR_EMPTY_ERR;
	}

	//Check if asking for data out of bounds
	if (pos >= v->length) {
		return VECTOR_OUT_OF_BOUNDS_ERR;
	}

	//Allocate space and clear buffer;
	if (realloc(data, v->data_size) == NULL) {
		return VECTOR_NULL_ERR;
	}
	memset(data, 0, v->data_size);

	//Get value
	memcpy(data, v->vector + (pos * v->data_size), v->data_size);

	return VECTOR_SUCCESS;
}


//Start vector
int vector_ini(vector_t * v, size_t data_size) {

	//Check NULL
	if (v == NULL || data_size == 0) {
		return VECTOR_NULL_ERR;
	}

	v->vector = malloc(0);
	v->data_size = data_size;
	v->length = 0;

	return VECTOR_SUCCESS;
}


//End vector, free memory
int vector_end(vector_t * v) {

	//Check NULL
	if (v == NULL) {
		return VECTOR_NULL_ERR;
	}

	if (v->length > 0) {
		free(v->vector);
	}

	return VECTOR_SUCCESS;
}
