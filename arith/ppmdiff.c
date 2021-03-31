/* Brian Huntley
 * CSC 411 - Arith
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

float rgb_difference_sum(Pnm_rgb pix1, Pnm_rgb pix2);

int main(int argc, char* argv[]){
	if(argc != 3){
		fprintf(stderr, "Incorrect args\n");
		exit(1);
	}

	FILE *fp1 = NULL;
	FILE *fp2 = NULL;

	fp1 = fopen(argv[1], "r");
	fp2 = fopen(argv[2], "r");

	if(fp1 == NULL || fp2 == NULL){
		fprintf(stderr, "Bad files\n");
		fclose(fp1);
		fclose(fp2);
		exit(1);
	}

	A2Methods_T methods = array2_methods_plain;
	Pnm_ppm img1 = Pnm_ppmread(fp1, methods);
	Pnm_ppm img2 = Pnm_ppmread(fp2, methods);

	if(img1->width != img2->width && (img1->width)-1 != img2->width && (img1->width)+1 != img2->width){
		fprintf(stderr, "Bad width\n");
		printf("1.0\n");
		exit(1);
	}

	if(img1->height != img2->height && (img1->height)-1 != img2->height && (img1->height)+1 != img2->height){
		fprintf(stderr, "Bad height\n");
		printf("1.0\n");
		exit(1);
	}

	float sum = 0;
	int low_height;
	int low_width;
	if(img1->height <= img2->height){
		low_height = img1->height;
	}else{
		low_height = img2->height;
	}

	if(img1->width <= img2->width){
		low_width = img1->width;
	}else{
		low_width = img2->width;
	}

	for(int r = 0; r < low_height; ++r){
		for(int c = 0; c < low_width; ++c){
			sum += rgb_difference_sum(methods->at(img1->pixels, c, r), methods->at(img2->pixels, c, r));
		}
	}
	sum /= (3 * low_width * low_height);
	sum = sqrtf(sum);

	printf("%1.4f\n", sum);

	Pnm_ppmfree(&img1);
	Pnm_ppmfree(&img2);
	fclose(fp1);
	fclose(fp2);

	return 0;
}

float rgb_difference_sum(Pnm_rgb pix1, Pnm_rgb pix2){
	float r = ((float) (pix1->red) - (float) (pix2->red)) / 255.0;
	r *= r;

	float g = ((float) (pix1->green) - (float) (pix2->green)) / 255.0;
	g *= g;

	float b = ((float) (pix1->blue) - (float) (pix2->blue)) / 255.0;
	b *= b;

	return (r + g + b);
}
