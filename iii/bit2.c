#include <stdlib.h>

#include "bit.h"
#include "bit2.h"
#include "assert.h"
#include "mem.h"
#include "array.h"

#define T Bit2_T

struct T {
	int width;
	int height;
	Array_T arr;
};

extern T Bit2_new(int width, int height){
	T bit2;
	NEW(bit2);
	assert(width > 0 && height > 0);
	bit2->width = width;
	bit2->height = height;
	bit2->arr = Array_new(width, sizeof(Array_T));

	for(int i = 0; i < width; ++i){
		Bit_T *tmp = Array_get(bit2->arr, i);
		*tmp = Bit_new(height);
	}
	return bit2;
}

extern void Bit2_free(T *bit2){
	assert(bit2 && *bit2);
	for(int i = 0; i < Bit2_width(*bit2); ++i){
		Bit_T *p = Array_get((*bit2)->arr, i);
		Bit_free(p);
	}
	Array_free(&(*bit2)->arr);
	FREE(*bit2);
}

extern int Bit2_width(T bit2){
	assert(bit2);
	return bit2->width;
}

extern int Bit2_height(T bit2){
	assert(bit2);
	return bit2->height;
}

extern int Bit2_put(T bit2, int col, int row, int val){
	assert(bit2);
	assert(row >= 0 && row < bit2->height);
	assert(col >= 0 && col < bit2->width);
	assert(val == 0 || val == 1);
	Bit_T *p = Array_get(bit2->arr, col);
	Bit_T inner = *p;
	return Bit_put(inner, row, val);
}

extern int Bit2_get(T bit2, int col, int row){
	assert(bit2);
	assert(row >= 0 && row < bit2->height);
	assert(col >= 0 && col < bit2->width);
	Bit_T *p = Array_get(bit2->arr, col);
	Bit_T inner = *p;
	return Bit_get(inner, row);
}

extern void Bit2_map_row_major(T bit2, void apply(int col, int row, int val, void *cl), void *cl){
	assert(bit2);
	for(int i = 0; i < bit2->height; ++i){
		for(int j = 0; j < bit2->width; ++j){
			int bit = Bit2_get(bit2, j, i);
			apply(j, i, bit, cl);
		}
	}
}

extern void Bit2_map_col_major(T bit2, void apply(int col, int row, int val, void *cl), void *cl){
	assert(bit2);
	for(int i = 0; i < bit2->width; ++i){
		for(int j = 0; j < bit2->height; ++j){
			int bit = Bit2_get(bit2, i, j);
			apply(i, j, bit, cl);
		}
	}
}

#undef T
