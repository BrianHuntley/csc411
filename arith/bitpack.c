/* Brian Huntley
 * CSC 411 - Arith
 */

#include "bitpack.h"
#include "assert.h"
#include "stdio.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

static inline uint64_t left_shift(uint64_t word, unsigned shift){
	assert(shift <= 64);
	if(shift == 64){
		return 0;
	}else{
		return word << shift;
	}
}

static inline uint64_t right_shift(uint64_t word, unsigned shift){
	assert(shift <= 64);
	if(shift == 64){
		return 0;
	}else{
		return word >> shift;
	}
}

static inline int64_t left_shift_signed(int64_t word, unsigned shift){
	assert(shift <= 64);
	if(shift == 64){
		return 0;
	}else{
		return word << shift;
	}
}

static inline int64_t right_shift_signed(int64_t word, unsigned shift){
	assert(shift <= 64);
	if(shift == 64){
		return 0;
	}else{
		return word >> shift;
	}
}

bool Bitpack_fitsu(uint64_t n, unsigned width){
	assert(width <= 64);
	if(width == 0){
		return false;
	}
	if((left_shift((uint64_t)1, width) - 1) < n ){
		return false;
	}
	return true;
}

bool Bitpack_fitss(int64_t n, unsigned width){
	assert(width <= 64);
	if(width <= 1){
		return false;
	}
	if(n < 0){
		n *= -1;
	}
	uint64_t abs_val = (uint64_t) n;
	return Bitpack_fitsu(abs_val, width - 1);
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb){
	assert(width <= 64);
	assert(width + lsb <= 64);
	uint64_t ls = left_shift(word, (64 - (width + lsb)));
	uint64_t rs = right_shift(ls, (64 - width));
	return rs;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb){
	assert(width <= 64);
	assert(width + lsb <= 64);
	int64_t ls = left_shift_signed(word, (64 - (width + lsb)));
	int64_t rs = right_shift_signed(ls, (64 - width));
	return rs;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value){
	assert(width <= 64);
	assert(width + lsb <= 64);
	if(!Bitpack_fitsu(value, width)){
		RAISE(Bitpack_Overflow);
	}
	uint64_t left_end = left_shift(right_shift(word, (width + lsb)),  (width + lsb));
	uint64_t right_end = right_shift(left_shift(word, (64 - lsb)), (64 - lsb));
	uint64_t combined = left_end | right_end;
	return combined | left_shift(value, lsb);
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, int64_t value){
	assert(width <= 64);
	assert(width + lsb <= 64);
	if(!Bitpack_fitss(value, width)){
		RAISE(Bitpack_Overflow);
	}
	uint64_t left_end = left_shift_signed(right_shift_signed(word, (width + lsb)),  (width + lsb));
	uint64_t right_end = right_shift_signed(left_shift_signed(word, (64 - lsb)), (64 - lsb));
	uint64_t combined = left_end | right_end;
	uint64_t u_value = Bitpack_getu(value, width, 0);
	return combined | left_shift(u_value, lsb);
}
