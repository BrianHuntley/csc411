/* Brian Huntley
 * CSC 411 - locality - Part C
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

void rotate_0(int c, int r, A2Methods_Array2 array2, void *val, void *cl);
void rotate_90(int c, int r, A2Methods_Array2 array2, void *val, void *cl);
void rotate_180(int c, int r, A2Methods_Array2 array2, void *val, void *cl);
void rotate_270(int c, int r, A2Methods_Array2 array2, void *val, void *cl);
void flip_horz(int c, int r, A2Methods_Array2 array2, void *val, void *cl);
void flip_vert(int c, int r, A2Methods_Array2 array2, void *val, void *cl);
void transpose(int c, int r, A2Methods_Array2 array2, void *val, void *cl);

int main(int argc, char *argv[]) {
	int rotation = 0;
	int flip = 0;
	int trans = 0;
	A2Methods_T methods = array2_methods_plain; // default to UArray2 methods
	assert(methods);
	A2Methods_mapfun *map = methods->map_default; // default to best map
	assert(map);
#define SET_METHODS(METHODS, MAP, WHAT) do { \
	methods = (METHODS); \
	assert(methods); \
	map = methods->MAP; \
	if (!map) { \
        	fprintf(stderr, "%s does not support " WHAT "mapping\n", argv[0]); \
        	exit(1); \
	} \
}while(0)
	int i;
	FILE *fp = NULL;
	for (i = 1; i < argc; i++) {
   		if (!strcmp(argv[i], "-row-major")) {
      			SET_METHODS(array2_methods_plain, map_row_major, "row-major");
    		} else if (!strcmp(argv[i], "-col-major")) {
      			SET_METHODS(array2_methods_plain, map_col_major, "column-major");
    		} else if (!strcmp(argv[i], "-block-major")) {
      			SET_METHODS(array2_methods_blocked, map_block_major, "block-major");
    		} else if (!strcmp(argv[i], "-rotate")) {
      			assert(i + 1 < argc);
      			char *endptr;
      			rotation = strtol(argv[++i], &endptr, 10);
      			assert(*endptr == '\0'); // parsed all correctly
	      		assert(rotation == 0   || rotation == 90 || rotation == 180 || rotation == 270);
    		} else if(!strcmp(argv[i], "-flip")){
			assert(i + 1 < argc);
			if(!strcmp(argv[i + 1], "horizontal")){
				flip = 1;
				++i;
			}else if(!strcmp(argv[i + 1], "vertical")){
				flip = 2;
				++i;
			}else{
				fprintf(stderr, "Invalid flip\n");
				exit(1);
			}
		}else if(!strcmp(argv[1], "-transpose")){
			 trans = 1;
		}else if (*argv[i] == '-') {
      			fprintf(stderr, "%s: unknown option '%s'\n", argv[0], argv[i]);
      			exit(1);
    		}else if (argc - i > 2) {
      			fprintf(stderr, "Usage: %s [-rotate <angle>] [-{row,col,block}-major] [filename]\n", argv[0]);
      			exit(1);
    		} else {
      			break;
    		}
  	}
	if(i == argc - 1){
		fp = fopen(argv[4], "r");
	}else{
		fp = stdin;
	}

	Pnm_ppm img = Pnm_ppmread(fp, methods);
	Pnm_ppm edited_img = malloc(sizeof(*img));
	//Creates an edited img with the proper dimensions for the specified transformation,
	//then calls the corresponding function for that transformation.
	if(rotation == 0 && flip == 0 && trans == 0){
		edited_img->pixels = methods->new(img->width, img->height, sizeof(struct Pnm_rgb));
		edited_img->methods = methods;
		edited_img->denominator = img->denominator;
		edited_img->width = img->width;
		edited_img->height = img->height;
		map(img->pixels, rotate_0, edited_img);
	}else if(rotation == 90){
		edited_img->pixels = methods->new(img->height, img->width, sizeof(struct Pnm_rgb));
		edited_img->methods = methods;
		edited_img->denominator = img->denominator;
		edited_img->width = img->height;
		edited_img->height = img->width;
		map(img->pixels, rotate_90, edited_img);
	}else if(rotation == 180){
		edited_img->pixels = methods->new(img->width, img->height, sizeof(struct Pnm_rgb));
		edited_img->methods = methods;
		edited_img->denominator = img->denominator;
		edited_img->width = img->width;
		edited_img->height = img->height;
		map(img->pixels, rotate_180, edited_img);
	}else if(rotation == 270){
		edited_img->pixels = methods->new(img->height, img->width, sizeof(struct Pnm_rgb));
		edited_img->methods = methods;
		edited_img->denominator = img->denominator;
		edited_img->width = img->height;
		edited_img->height = img->width;
		map(img->pixels, rotate_270, edited_img);
	}else if(flip == 1){
		edited_img->pixels = methods->new(img->width, img->height, sizeof(struct Pnm_rgb));
		edited_img->methods = methods;
		edited_img->denominator = img->denominator;
		edited_img->width = img->width;
		edited_img->height = img->height;
		map(img->pixels, flip_horz, edited_img);
	}else if(flip == 2){
		edited_img->pixels = methods->new(img->width, img->height, sizeof(struct Pnm_rgb));
		edited_img->methods = methods;
		edited_img->denominator = img->denominator;
		edited_img->width = img->width;
		edited_img->height = img->height;
		map(img->pixels, flip_vert, edited_img);
	}else if(trans == 1){
		edited_img->pixels = methods->new(img->height, img->width, sizeof(struct Pnm_rgb));
		edited_img->methods = methods;
		edited_img->denominator = img->denominator;
		edited_img->width = img->height;
		edited_img->height = img->width;
		map(img->pixels, transpose, edited_img);
	}

	Pnm_ppmwrite(stdout, edited_img);

	fclose(fp);
	Pnm_ppmfree(&img);
	Pnm_ppmfree(&edited_img);
	return 0;
}

void rotate_0(int c, int r, A2Methods_Array2 array2, void *val, void *cl){
	//Copys original to edited
	(void) array2;
	Pnm_ppm edited = (Pnm_ppm) cl;
	Pnm_rgb edited_pixel = edited->methods->at(edited->pixels, c, r);
	Pnm_rgb original_pixel = (Pnm_rgb) val;
	*edited_pixel = *original_pixel;
}

void rotate_90(int c, int r, A2Methods_Array2 array2, void *val, void *cl){
	//(i, j) in original becomes (h - j - 1, i) in rotated
	(void) array2;
	Pnm_ppm edited = (Pnm_ppm) cl;
	Pnm_rgb edited_pixel = edited->methods->at(edited->pixels, ((edited->width) - r - 1), c);
	Pnm_rgb original_pixel = (Pnm_rgb) val;
	*edited_pixel = *original_pixel;
}

void rotate_180(int c, int r, A2Methods_Array2 array2, void *val, void *cl){
	//(i, j) in original becomes (w - i - 1, h - j - 1) in rotated
	(void) array2;
	Pnm_ppm edited = (Pnm_ppm) cl;
	Pnm_rgb edited_pixel = edited->methods->at(edited->pixels, (edited->width) - c - 1, (edited->height) - r - 1);
	Pnm_rgb original_pixel = (Pnm_rgb) val;
	*edited_pixel = *original_pixel;
}

void rotate_270(int c, int r, A2Methods_Array2 array2, void *val, void *cl){
	//(i, j) in original becomes (j, w - i - 1) in rotated
	(void) array2;
	Pnm_ppm edited = (Pnm_ppm) cl;
	Pnm_rgb edited_pixel = edited->methods->at(edited->pixels, r, (edited->height) - c - 1);
	Pnm_rgb original_pixel = (Pnm_rgb) val;
	*edited_pixel = *original_pixel;
}

void flip_horz(int c, int r, A2Methods_Array2 array2, void *val, void *cl){
	//(i, j) in original becomes (w - i - 1, j) in flipped
	(void) array2;
	Pnm_ppm edited = (Pnm_ppm) cl;
	Pnm_rgb edited_pixel = edited->methods->at(edited->pixels, (edited->width) - c - 1, r);
	Pnm_rgb original_pixel = (Pnm_rgb) val;
	*edited_pixel = *original_pixel;
}

void flip_vert(int c, int r, A2Methods_Array2 array2, void *val, void *cl){
	//(i, j) in original becomes (i, h - j - 1) in flipped
	(void) array2;
	Pnm_ppm edited = (Pnm_ppm) cl;
	Pnm_rgb edited_pixel = edited->methods->at(edited->pixels, c, (edited->height) - r - 1);
	Pnm_rgb original_pixel = (Pnm_rgb) val;
	*edited_pixel = *original_pixel;
}

void transpose(int c, int r, A2Methods_Array2 array2, void *val, void *cl){
	//rows of original turn into columns of edited
	(void) array2;
	Pnm_ppm edited = (Pnm_ppm) cl;
	Pnm_rgb edited_pixel = edited->methods->at(edited->pixels, r, c);
	Pnm_rgb original_pixel = (Pnm_rgb) val;
	*edited_pixel = *original_pixel;
}
