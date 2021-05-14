/* Brian Huntley
 * CSC 411 - Universal Machine
 */

#include "seq.h"

void execute_program(Seg_T program);

void conditional_move(uint32_t *registers, int a, int b, int c);

void segmented_load(uint32_t *registers, Seg_T outer,  int a, int b, int c);

void segmented_store(uint32_t *registers, Seg_T outer,int a, int b, int c);

void addition(uint32_t *registers, int a, int b, int c);

void multiplication(uint32_t *registers, int a, int b, int c);

void division(uint32_t *registers, int a, int b, int c);

void bitwise_nand(uint32_t *registers, int a, int b, int c);

void halt(Seg_T outer);

void map_segment(uint32_t *registers, Seg_T outer, int b, int c);

void unmap_segment(uint32_t *registers, Seg_T outer, int c);

void output(uint32_t *registers, int c);

void input(uint32_t *registers, int c);

void load_program(uint32_t *registers, Seg_T outer, int b);

void load_value(uint32_t *registers, int a, uint32_t val);
