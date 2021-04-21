/* Brian Huntley
 * CSC 411 - Universal Machine
 */

#include <stdio.h>
#include <string.h>

#include "load.h"
#include "seg.h"
#include "bitpack.h"

/* Loads the program from fp and puts it into a segment. */
Seg_T read_program(FILE *fp){
	//Find number of instructions using size of file
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp) / sizeof(uint32_t);
	rewind(fp);

	Seg_T outer = Seg_new();

	Segment *tmp = malloc(sizeof(struct Segment) + (size * sizeof(uint32_t)));
	tmp->size = size;
	memset(tmp->vals, 0, (size * sizeof(uint32_t)));

	//store words in segment
	for(int i = 0; i < size; ++i){
		uint32_t word = 0;
		for(int j = 3; j >= 0; --j){
			word = Bitpack_newu(word, 8, (j * 8), fgetc(fp));
		}
		tmp->vals[i] = word;
	}
	Seq_addhi(outer->segments, tmp);

	return outer;
}

