/* Brian Huntley
 * CSC 411 - iii - Part A
 */
#include <stdlib.h>

#include "array.h"
#include "uarray2.h"
#include "assert.h"
#include "mem.h"

#define T UArray2_T

struct T {
	int width;
	int height;
	int size;
	Array_T arr;
};

extern T UArray2_new(int width, int height, int size){
	T uarray2;
	NEW(uarray2);
	assert(width > 0 && height > 0 && size > 0);
	uarray2->width = width;
	uarray2->height = height;
	uarray2->size = size;
	uarray2->arr = Array_new(width, sizeof(Array_T));

	for(int i = 0; i < width; ++i){
		Array_T *tmp = Array_get(uarray2->arr, i);
		*tmp = Array_new(height, size);
	}

	return uarray2;
}

extern void UArray2_free(T *uarray2){
	assert(uarray2 && *uarray2);
	for(int i = 0; i < UArray2_width(*uarray2); ++i){
		Array_T *p = Array_get((*uarray2)->arr, i);
		Array_free(p);
	}
	Array_free(&((*uarray2)->arr));
	FREE(*uarray2);
}

extern int UArray2_width(T uarray2){
	assert(uarray2);
	return uarray2->width;
}

extern int UArray2_height(T uarray2){
	assert(uarray2);
	return uarray2->height;
}

extern int UArray2_size(T uarray2){
	assert(uarray2);
	return uarray2->size;
}

extern void *UArray2_at(T uarray2, int col, int row){
	assert(uarray2);
	assert(row >= 0 && row < uarray2->height);
	assert(col >= 0 && col < uarray2->width);
	Array_T *p = Array_get(uarray2->arr, col);
	Array_T inner = *p;
	return Array_get(inner, row);
}

extern void UArray2_map_row_major(T uarray2, void apply(int col, int row, T uarray2, void *val, void *cl), void *cl){
	assert(uarray2);
	for(int i = 0; i < uarray2->height; ++i){
		for(int j = 0; j < uarray2->width; ++j){
			Array_T *p = Array_get(uarray2->arr, j);
			Array_T inner_array = *p;
			apply(j, i, uarray2, Array_get(inner_array, i), cl);
		}
	}
}

extern void UArray2_map_col_major(T uarray2, void apply(int col, int row, T uarray2, void *val, void *cl), void *cl){
	assert(uarray2);
	for(int i = 0; i < uarray2->width; ++i){
		Array_T *p = Array_get(uarray2->arr, i);
		Array_T curr = *p;
		for(int j = 0; j < uarray2->height; ++j){
			apply(i, j, uarray2, Array_get(curr, j), cl);
		}
	}
}

#undef T
