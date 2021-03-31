/* Brian Huntley
 * CSC 411 - Arith
 */

#include "algorithms.h"

A2Methods_Array2 rgb_to_component(A2Methods_Array2 pixels, float denominator){

	A2Methods_T methods = array2_methods_plain;
	A2Methods_Array2 component_arr = methods->new(methods->width(pixels), methods->height(pixels), sizeof(struct Component_video));

	for(int r = 0; r < methods->height(pixels); ++r){
		for(int c = 0; c < methods->width(pixels); ++c){
			Component_video *comp = methods->at(component_arr, c, r);
			Pnm_rgb pixel = methods->at(pixels, c, r);

			float r = pixel->red / denominator;
			float g = pixel->green / denominator;
			float b = pixel->blue / denominator;

			//Arithmetic provided in assignment handout.
			comp->y = 0.299 * r + 0.587 * g + 0.114 * b;
			comp->pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
			comp->pr = 0.5 * r - 0.418688 * g - 0.081312 * b;
		}
	}
	return component_arr;
}

A2Methods_Array2 component_to_rgb(A2Methods_Array2 component_arr){

	A2Methods_T methods = array2_methods_plain;
	A2Methods_Array2 pixels = methods->new(methods->width(component_arr), methods->height(component_arr), sizeof(struct Pnm_rgb));

	for(int r = 0; r < methods->height(component_arr); ++r){
		for(int c = 0; c < methods->width(component_arr); ++c){
			Component_video *component = methods->at(component_arr, c, r);
			Pnm_rgb pixel = methods->at(pixels, c, r);

			float y = component->y;
			float pb = component->pb;
			float pr = component->pr;

			//Arithmetic provided in assignment handout.
			float r = 1.0 * y + 0.0 * pb + 1.402 * pr;
			float g = 1.0 * y - 0.344136 * pb - 0.714136 * pr;
			float b = 1.0 * y + 1.772 * pb + 0.0 * pr;

			if(r < 0.0){
				r = 0.0;
			}
			if(r > 1.0){
				r = 1.0;
			}

			if(g < 0.0){
				g = 0.0;
			}
			if(g > 1.0){
				g = 1.0;
			}

			if(b < 0.0){
				b = 0.0;
			}
			if(b > 1.0){
				b = 1.0;
			}

			r *= 255;
			g *= 255;
			b *= 255;

			pixel->red = r;
			pixel->green = g;
			pixel->blue = b;
		}
	}
	return pixels;
}

A2Methods_Array2 dct(A2Methods_Array2 component_arr){
	A2Methods_T methods = array2_methods_plain;
	A2Methods_Array2 coeff_arr = methods->new(methods->width(component_arr)/2, methods->height(component_arr)/2, sizeof(struct Coefficients));
	for(int r = 0; r < methods->height(component_arr); r+=2){
		for(int c = 0; c < methods->width(component_arr); c+=2){
			Coefficients *curr = methods->at(coeff_arr, c/2, r/2);
			Component_video *y1 = methods->at(component_arr, c, r);
			Component_video *y2 = methods->at(component_arr, c + 1, r);
			Component_video *y3 = methods->at(component_arr, c, r + 1);
			Component_video *y4 = methods->at(component_arr, c + 1, r + 1);

			//Arithmetic provided in assignment handout.
			float a = (y4->y + y3->y + y2->y + y1->y) / 4.0;
			float b = (y4->y + y3->y - y2->y - y1->y) / 4.0;
			float c = (y4->y - y3->y + y2->y - y1->y) / 4.0;
			float d = (y4->y - y3->y - y2->y + y1->y) / 4.0;

			if(b < -0.3){
				b = -0.3;
			}
			if(b > 0.3){
				b = 0.3;
			}

			if(c < -0.3){
				c = -0.3;
			}
			if(c > 0.3){
				c = 0.3;
			}

			if(d < -0.3){
				d = -0.3;
			}
			if(d > 0.3){
				d = 0.3;
			}

			curr->a = (unsigned int)round(511.0 * a);
			curr->b = (signed int)round(50.0 * b);
			curr->c = (signed int)round(50.0 * c);
			curr->d = (signed int)round(50.0 * d);

			assert(a >= 0 && a <= 511);
			assert(b >= -15 && b <= 15);
			assert(c >= -15 && c <= 15);
			assert(d >= -15 && d <= 15);

			float pb = (y4->pb + y3->pb + y2->pb + y1->pb) / 4.0;
			float pr = (y4->pr + y3->pr + y2->pr + y1->pr) / 4.0;

			curr->pb = Arith_index_of_chroma(pb);
			curr->pr = Arith_index_of_chroma(pr);
		}
	}

	return coeff_arr;
}

A2Methods_Array2 inverse_dct(A2Methods_Array2 coeff_arr){
	A2Methods_T methods = array2_methods_plain;
	A2Methods_Array2 component_arr = methods->new(methods->width(coeff_arr)*2, methods->height(coeff_arr)*2, sizeof(struct Component_video));
	for(int r = 0; r < methods->height(component_arr); r+=2){
		for(int c = 0; c < methods->width(component_arr); c+=2){
			Coefficients *curr = methods->at(coeff_arr, c/2, r/2);
			Component_video *y1 = methods->at(component_arr, c, r);
			Component_video *y2 = methods->at(component_arr, c + 1, r);
			Component_video *y3 = methods->at(component_arr, c, r + 1);
			Component_video *y4 = methods->at(component_arr, c + 1, r + 1);

			float a = ((float)curr->a / 511.0);
			float b = ((float)curr->b / 50.0);
			float c = ((float)curr->c / 50.0);
			float d = ((float)curr->d / 50.0);

			//Arithmetic provided in assignment handout.
			y1->y = (a - b - c + d);
			y2->y = (a - b + c - d);
			y3->y = (a + b - c - d);
			y4->y = (a + b + c + d);

			float pb = Arith_chroma_of_index(curr->pb);
			float pr = Arith_chroma_of_index(curr->pr);

			y1->pb = pb;
			y1->pr = pr;

			y2->pb = pb;
			y2->pr = pr;

			y3->pb = pb;
			y3->pr = pr;

			y4->pb = pb;
			y4->pr = pr;
		}
	}

	return component_arr;
}

A2Methods_Array2 pack_coeffs(A2Methods_Array2 coeff_arr){
	A2Methods_T methods = array2_methods_plain;
	A2Methods_Array2 packed_arr = methods->new(methods->width(coeff_arr), methods->height(coeff_arr), sizeof(uint64_t));
	for(int r = 0; r < methods->height(packed_arr); ++r){
		for(int c = 0; c < methods->width(packed_arr); ++c){
			Coefficients *curr = methods->at(coeff_arr, c, r);
			uint64_t *word = methods->at(packed_arr, c, r);
			*word = 0;
			*word = Bitpack_newu(*word, 9, 23, curr->a);
			*word = Bitpack_news(*word, 5, 18, curr->b);
			*word = Bitpack_news(*word, 5, 13, curr->c);
			*word = Bitpack_news(*word, 5, 8, curr->d);
			*word = Bitpack_newu(*word, 4, 4, curr->pb);
			*word = Bitpack_newu(*word, 4, 0, curr->pr);
		}
	}
	return packed_arr;
}

A2Methods_Array2 unpack_coeffs(A2Methods_Array2 packed_arr){
	A2Methods_T methods = array2_methods_plain;
	A2Methods_Array2 coeff_arr = methods->new(methods->width(packed_arr), methods->height(packed_arr), sizeof(struct Coefficients));
	for(int r = 0; r < methods->height(packed_arr); ++r){
		for(int c = 0; c < methods->width(packed_arr); ++c){
			Coefficients *curr = methods->at(coeff_arr, c, r);
			uint64_t *word = methods->at(packed_arr, c, r);
			curr->a = Bitpack_getu(*word, 9, 23);
			curr->b = Bitpack_gets(*word, 5, 18);
			curr->c = Bitpack_gets(*word, 5, 13);
			curr->d = Bitpack_gets(*word, 5, 8);
			curr->pb = Bitpack_getu(*word, 4, 4);
			curr->pr = Bitpack_getu(*word, 4, 0);
		}
	}
	return coeff_arr;
}

void print_packed_values(A2Methods_Array2 packed_arr){
	A2Methods_T methods = array2_methods_plain;
	printf("Compressed image format 2\n%u %u", methods->width(packed_arr), methods->height(packed_arr));
	printf("\n");
	for(int r = 0; r < methods->height(packed_arr); ++r){
		for(int c = 0; c < methods->width(packed_arr); ++c){
			uint64_t *curr = methods->at(packed_arr, c, r);
			putchar(Bitpack_getu(*curr, 8, 24));
			putchar(Bitpack_getu(*curr, 8, 16));
			putchar(Bitpack_getu(*curr, 8, 8));
			putchar(Bitpack_getu(*curr, 8, 0));
		}
	}
}

A2Methods_Array2 read_packed_values(FILE *fp){
	A2Methods_T methods = array2_methods_plain;
	unsigned height, width;
	int read = fscanf(fp, "Compressed image format 2\n%u %u", &width, &height);
	assert(read == 2);
	int c = getc(fp);
	assert(c == '\n');
	A2Methods_Array2 packed_arr = methods->new(width, height, sizeof(uint64_t));
	for(int r = 0; r < methods->height(packed_arr); ++r){
		for(int c = 0; c < methods->width(packed_arr); ++c){
			uint64_t *word = methods->at(packed_arr, c, r);
			*word = Bitpack_newu(*word, 8, 24, getc(fp));
			*word = Bitpack_newu(*word, 8, 16, getc(fp));
			*word = Bitpack_newu(*word, 8, 8, getc(fp));
			*word = Bitpack_newu(*word, 8, 0, getc(fp));
		}
	}
	return packed_arr;
}
