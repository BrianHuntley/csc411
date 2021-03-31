/* Brian Huntley
 * CSC 411 - Arith
 */

#include <stdio.h>
#include <stdlib.h>

#include "compress.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

#include "algorithms.h"

void copy(int c, int r, A2Methods_Array2 array2, void *val, void *cl){
	(void) array2;
	A2Methods_Array2 original = (A2Methods_Array2) cl;
	A2Methods_T methods = array2_methods_plain;
	*(Pnm_rgb) val = *(Pnm_rgb) methods->at(original, c, r);
}

void compress(FILE *input){
	A2Methods_T methods = array2_methods_plain;
	Pnm_ppm img = Pnm_ppmread(input, methods);

	//check if img needs to be trimmed
	unsigned int width = img->width;
	unsigned int height = img->height;
	if(width % 2){
		--width;
	}
	if(height % 2){
		--height;
	}
	A2Methods_Array2 trimmed = methods->new(width, height, sizeof(struct Pnm_rgb));
	methods->map_col_major(trimmed, copy, img->pixels);
	methods->free(&img->pixels);
	img->pixels = trimmed;
	img->width = width;
	img->height = height;
	//end trimming

	A2Methods_Array2 component_arr = rgb_to_component(img->pixels, img->denominator);
	Pnm_ppmfree(&img);
	A2Methods_Array2 coeff_arr = dct(component_arr);
	methods->free(&component_arr);
	A2Methods_Array2 packed_arr = pack_coeffs(coeff_arr);
	methods->free(&coeff_arr);
	print_packed_values(packed_arr);
	methods->free(&packed_arr);
}

void decompress(FILE *input){
	A2Methods_T methods = array2_methods_plain;

	A2Methods_Array2 packed_arr = read_packed_values(input);
	A2Methods_Array2 unpacked_arr = unpack_coeffs(packed_arr);
	methods->free(&packed_arr);
	A2Methods_Array2 component_arr = inverse_dct(unpacked_arr);
	methods->free(&unpacked_arr);
	A2Methods_Array2 pixel_arr = component_to_rgb(component_arr);
	methods->free(&component_arr);

	Pnm_ppm img = malloc(sizeof(*img));
	img->pixels = pixel_arr;
	img->methods = methods;
	img->denominator = 255;
	img->width = methods->width(pixel_arr);
	img->height = methods->height(pixel_arr);;

	Pnm_ppmwrite(stdout, img);
	Pnm_ppmfree(&img);
}
