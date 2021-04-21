/* Brian Huntley
 * CSC 411 - Universal Machine
 */

#include <stdio.h>

#include "assert.h"
#include "seg.h"
#include "bitpack.h"
#include "execute.h"

/* Executes the instructions that are contained in the segments. */
void execute_program(Seg_T program){
	uint32_t registers[8] = {0};
	int i = 0;
	//keep getting instructions and running them until halt.
	while(1){
		Segment *tmp = Seq_get(program->segments, 0);
		uint32_t curr = tmp->vals[i];
		int instruction_id = Bitpack_getu(curr, 4, 28);
		int a, b, c;
		switch(instruction_id){
			case 0:
				a = Bitpack_getu(curr, 3, 6);
				b = Bitpack_getu(curr, 3, 3);
				c = Bitpack_getu(curr, 3, 0);
				conditional_move(registers, a, b, c);
				break;
			case 1:
				a = Bitpack_getu(curr, 3, 6);
				b = Bitpack_getu(curr, 3, 3);
				c = Bitpack_getu(curr, 3, 0);
				segmented_load(registers, program, a, b, c);
				break;
			case 2:
				a = Bitpack_getu(curr, 3, 6);
				b = Bitpack_getu(curr, 3, 3);
				c = Bitpack_getu(curr, 3, 0);
				segmented_store(registers, program, a, b, c);
				break;
			case 3:
				a = Bitpack_getu(curr, 3, 6);
				b = Bitpack_getu(curr, 3, 3);
				c = Bitpack_getu(curr, 3, 0);
				addition(registers, a, b, c);
				break;
			case 4:
				a = Bitpack_getu(curr, 3, 6);
				b = Bitpack_getu(curr, 3, 3);
				c = Bitpack_getu(curr, 3, 0);
				multiplication(registers, a, b, c);
				break;
			case 5:
				a = Bitpack_getu(curr, 3, 6);
				b = Bitpack_getu(curr, 3, 3);
				c = Bitpack_getu(curr, 3, 0);
				division(registers, a, b, c);
				break;
			case 6:
				a = Bitpack_getu(curr, 3, 6);
				b = Bitpack_getu(curr, 3, 3);
				c = Bitpack_getu(curr, 3, 0);
				bitwise_nand(registers, a, b, c);
				break;
			case 7:
				halt(program);
				break;
			case 8:
				b = Bitpack_getu(curr, 3, 3);
				c = Bitpack_getu(curr, 3, 0);
				map_segment(registers, program, b, c);
				break;
			case 9:
				c = Bitpack_getu(curr, 3, 0);
				unmap_segment(registers, program, c);
				break;
			case 10:
				c = Bitpack_getu(curr, 3, 0);
				output(registers, c);
				break;
			case 11:
				c = Bitpack_getu(curr, 3, 0);
				input(registers, c);
				break;
			case 12:
				b = Bitpack_getu(curr, 3, 3);
				c = Bitpack_getu(curr, 3, 0);
				load_program(registers, program, &i, b, c);
				break;
			case 13:
				a = Bitpack_getu(curr, 3, 25);
				uint32_t val = Bitpack_getu(curr, 25, 0);
				load_value(registers, a, val);
				break;
		}
		++i;
	}
}
/* If register[c] != 0 then registers[a] = registers[b] */
void conditional_move(uint32_t *registers, int a, int b, int c){
	if(registers[c] != 0){
		registers[a] = registers[b];
	}
}

/* Loads m[r[b]r[c]] into r[a] */
void segmented_load(uint32_t *registers, Seg_T outer, int a, int b, int c){
	Segment *tmp = Seq_get(outer->segments, registers[b]);
	registers[a] = tmp->vals[registers[c]];
}

/* Stores m[r[a]r[b]] into r[c] */
void segmented_store(uint32_t *registers, Seg_T outer, int a, int b, int c){
	Segment *tmp = Seq_get(outer->segments, registers[a]);
	tmp->vals[registers[b]] = registers[c];
}

/* Sums r[b] and r[c], storing the value into r[a] */
void addition(uint32_t *registers, int a, int b, int c){
	registers[a] = registers[b] + registers[c];
}

/* Multiplies r[b] by r[c], storing the value into r[a] */
void multiplication(uint32_t *registers, int a, int b, int c){
	registers[a] = registers[b] * registers[c];
}

/* Divides r[b] by r[c], storing the value into r[a] */
void division(uint32_t *registers, int a, int b, int c){
	registers[a] = registers[b] / registers[c];
}

/* Stores the bitwise NAND of r[b]&r[c] into r[a] */
void bitwise_nand(uint32_t *registers, int a, int b, int c){
	registers[a] = ~(registers[b] & registers[c]);
}

/* Frees the memory and ends the computation */
void halt(Seg_T outer){
	Seg_free(&outer);
	exit(0);
}

/* Creates a new segment of size r[c], mapped into m[r[b]]. */
void map_segment(uint32_t *registers, Seg_T outer, int b, int c){
	registers[b] = Seg_map(outer, registers[c]);
}

/* Unmaps the segment at m[r[c]]. */
void unmap_segment(uint32_t *registers, Seg_T outer, int c){
	Seg_unmap(outer, registers[c]);
}

/* Prints the character at r[c] to stdout. */
void output(uint32_t *registers, int c){
	printf("%c", registers[c]);
}

/* Gets input from stdin, storing the character at r[c] */
void input(uint32_t *registers, int c){
	int in = fgetc(stdin);
	if(in == EOF){
		uint32_t all_1s = ~0;
		registers[c] = all_1s;
	}else{
		assert(in >= 0 && in <= 255);
		registers[c] = in;
	}
}

/* Loads m[r[b]] into m[0]. Starts the segment 0 at r[c]. */
void load_program(uint32_t *registers, Seg_T outer, int *i, int b, int c){
	Segment *tmp = Seq_get(outer->segments, registers[b]);
	Seq_put(outer->segments, 0, tmp);
	*i = (int) registers[c] - 1;
}

/* Loads the given value into r[a]. */
void load_value(uint32_t *registers, int a, uint32_t val){
	registers[a] = val;
}
