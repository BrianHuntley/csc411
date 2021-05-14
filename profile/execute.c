/* Brian Huntley
 * CSC 411 - Universal Machine
 */

#include <string.h>
#include <stdio.h>

#include "assert.h"
#include "seg.h"
#include "execute.h"

/* Executes the instructions that are contained in the segments. */
void execute_program(Seg_T program){
	uint32_t registers[8] = {0};
	int i = 0;
	Segment *tmp = Seq_get(program->segments, 0);
	//keep getting instructions and running them until halt.
	while(1){
		uint32_t curr = tmp->vals[i];
		int instruction_id = (curr >> 28);
		int a, b, c;
		switch(instruction_id){
			case 0:
				a = ((curr << 23) >> 29);
				b = ((curr << 26) >> 29);
				c = ((curr << 29) >> 29);
				conditional_move(registers, a, b, c);
				break;
			case 1:
                                a = ((curr << 23) >> 29);
                                b = ((curr << 26) >> 29);
                                c = ((curr << 29) >> 29);
				if(!registers[b]){
					registers[a] = tmp->vals[registers[c]];
				}else{
					segmented_load(registers, program, a, b, c);
				}
				break;
			case 2:
                                a = ((curr << 23) >> 29);
                                b = ((curr << 26) >> 29);
                                c = ((curr << 29) >> 29);
				if(!registers[a]){
					tmp->vals[registers[b]] = registers[c];
				}else{
					segmented_store(registers, program, a, b, c);
				}
				break;
			case 3:
                                a = ((curr << 23) >> 29);
                                b = ((curr << 26) >> 29);
                                c = ((curr << 29) >> 29);
				addition(registers, a, b, c);
				break;
			case 4:
                                a = ((curr << 23) >> 29);
                                b = ((curr << 26) >> 29);
                                c = ((curr << 29) >> 29);
				multiplication(registers, a, b, c);
				break;
			case 5:
                                a = ((curr << 23) >> 29);
                                b = ((curr << 26) >> 29);
                                c = ((curr << 29) >> 29);
				division(registers, a, b, c);
				break;
			case 6:
                                a = ((curr << 23) >> 29);
                                b = ((curr << 26) >> 29);
                                c = ((curr << 29) >> 29);
				bitwise_nand(registers, a, b, c);
				break;
			case 7:
				halt(program);
				break;
			case 8:
                                b = ((curr << 26) >> 29);
                                c = ((curr << 29) >> 29);
				map_segment(registers, program, b, c);
				break;
			case 9:
                                c = ((curr << 29) >> 29);
				unmap_segment(registers, program, c);
				break;
			case 10:
                                c = ((curr << 29) >> 29);
				output(registers, c);
				break;
			case 11:
                                c = ((curr << 29) >> 29);
				input(registers, c);
				break;
			case 12:
                                b = ((curr << 26) >> 29);
				c = ((curr << 29) >> 29);
				if(registers[b]){
					load_program(registers, program, b);
					tmp = (Segment*) Seq_get(program->segments, 0);
				}
				i = registers[c] - 1;
				break;
			case 13:
				a = ((curr << 4) >> 29);
				uint32_t val = ((curr << 7) >> 7);
				load_value(registers, a, val);
				break;
		}
		++i;
	}
}
/* If register[c] != 0 then registers[a] = registers[b] */
inline void conditional_move(uint32_t *registers, int a, int b, int c){
	if(registers[c] != 0){
		registers[a] = registers[b];
	}
}

/* Loads m[r[b]r[c]] into r[a] */
inline void segmented_load(uint32_t *registers, Seg_T outer, int a, int b, int c){
	Segment *tmp = Seq_get(outer->segments, registers[b]);
	registers[a] = tmp->vals[registers[c]];
}

/* Stores r[c] into m[r[a]r[b]] */
inline void segmented_store(uint32_t *registers, Seg_T outer, int a, int b, int c){
	Segment *tmp = Seq_get(outer->segments, registers[a]);
	tmp->vals[registers[b]] = registers[c];
}

/* Sums r[b] and r[c], storing the value into r[a] */
inline void addition(uint32_t *registers, int a, int b, int c){
	registers[a] = registers[b] + registers[c];
}

/* Multiplies r[b] by r[c], storing the value into r[a] */
inline void multiplication(uint32_t *registers, int a, int b, int c){
	registers[a] = registers[b] * registers[c];
}

/* Divides r[b] by r[c], storing the value into r[a] */
inline void division(uint32_t *registers, int a, int b, int c){
	registers[a] = registers[b] / registers[c];
}

/* Stores the bitwise NAND of r[b]&r[c] into r[a] */
inline void bitwise_nand(uint32_t *registers, int a, int b, int c){
	registers[a] = ~(registers[b] & registers[c]);
}

/* Frees the memory and ends the computation */
inline void halt(Seg_T outer){
	Seg_free(&outer);
	exit(0);
}

/* Creates a new segment of size r[c], mapped into m[r[b]]. */
inline void map_segment(uint32_t *registers, Seg_T outer, int b, int c){
	registers[b] = Seg_map(outer, registers[c]);
}

/* Unmaps the segment at m[r[c]]. */
inline void unmap_segment(uint32_t *registers, Seg_T outer, int c){
	Seg_unmap(outer, registers[c]);
}

/* Prints the character at r[c] to stdout. */
inline void output(uint32_t *registers, int c){
	printf("%c", registers[c]);
}

/* Gets input from stdin, storing the character at r[c] */
inline void input(uint32_t *registers, int c){
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
inline void load_program(uint32_t *registers, Seg_T outer, int b){
	free(Seq_get(outer->segments, 0));
	Segment *original = Seq_get(outer->segments, registers[b]);
	uint32_t size = original->size;
	Segment *copy = malloc(sizeof(struct Segment) + (size * sizeof(uint32_t)));
	copy->size = size;
	memcpy(copy->vals, original->vals, (size * sizeof(uint32_t)));
	Seq_put(outer->segments, 0, copy);

}

/* Loads the given value into r[a]. */
inline void load_value(uint32_t *registers, int a, uint32_t val){
	registers[a] = val;
}

