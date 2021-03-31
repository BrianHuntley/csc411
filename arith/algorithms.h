/* Brian Huntley
 * CSC 411 - Arith
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "a2methods.h"
#include "a2blocked.h"
#include "a2plain.h"
#include "pnm.h"
#include "assert.h"
#include "arith411.h"
#include "bitpack.h"

typedef struct Component_video {
	float y;
	float pb;
	float pr;
} Component_video;

typedef struct Coefficients {
	unsigned int a;
	signed int b;
	signed int c;
	signed int d;
	unsigned int pb;
	unsigned int pr;
} Coefficients;

//Converts a 2d array of Pnm_rgb pixel values to a 2d array of Component_video.
A2Methods_Array2 rgb_to_component(A2Methods_Array2 pixels, float denominator);

//Converts a 2d array of Component_video values to a 2d array of Pnm_rgb values.
A2Methods_Array2 component_to_rgb(A2Methods_Array2 component_arr);

//Performs the discrete cosine transform on an array of Component_video values and stores them in a 2d array of Coefficients.
A2Methods_Array2 dct(A2Methods_Array2 component_arr);

//Performs the inverse discrete cosine transform on a 2d array of Coefficients and stores them in a 2d array of Component_video.
A2Methods_Array2 inverse_dct(A2Methods_Array2 coeff_arr);

//Packs a 2d array of coefficients as specified in the assignment handout using Bitpack.
A2Methods_Array2 pack_coeffs(A2Methods_Array2 coeff_arr);

//Retrieves coefficients from a 2d array of packed values.
A2Methods_Array2 unpack_coeffs(A2Methods_Array2 packed_arr);

//Prints to stdout the packed values in the format specified in the assignment handout.
void print_packed_values(A2Methods_Array2 packed_arr);

//Reads a file containing an image of the format specified in the assignment handout.
A2Methods_Array2 read_packed_values(FILE *fp);
