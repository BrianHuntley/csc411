/* Brian Huntley
 * CSC 411 - Universal Machine
 */

#ifndef SEG_INCLUDED
#define SEG_INCLUDED

#include <stdlib.h>
#include <stdint.h>

#include <stack.h>
#include "seq.h"

struct Seg_T{
        Seq_T segments;
        Stack_T unmapped;
};

typedef struct Segment{
	uint32_t size;
	uint32_t vals[];
} Segment;

typedef struct Seg_T *Seg_T;

extern Seg_T Seg_new();

extern uint32_t Seg_map(Seg_T seg, uint32_t size);

extern void Seg_unmap(Seg_T seg, int idx);

extern void Seg_free(Seg_T *seg);

#undef T
#endif
