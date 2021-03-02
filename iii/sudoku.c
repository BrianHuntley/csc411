#include <stdlib.h>
#include <stdio.h>

#include "pnmrdr.h"
#include "except.h"
#include "uarray2.h"

Pnmrdr_T load_pnm(FILE *fp);
UArray2_T pnm_to_uarray2(Pnmrdr_T img);
void insert_pixels(int x, int y, void *val, void *cl);
void check_row(int x, int y, void *val, void *cl);
void check_col(int x, int y, void *val, void *cl);
void check_square(UArray2_T arr, void *repeats);

int main(int argc, char *argv[]){
	FILE *fp = NULL;
	//Input file coming from argv
	if(argc == 2){
		fp = fopen(argv[1], "r");
	}
	//Input coming from standard input
	else if(argc == 1){
		fp = stdin;
	}
	else{
		exit(1);
	}
	Pnmrdr_T img = load_pnm(fp);
	UArray2_T arr = pnm_to_uarray2(img);
	Pnmrdr_free(&img);
	fclose(fp);
	//keeps track of how many of each digit exist.
	//repeats[0] keeps track of how many errors are found.
	int repeats[10] = {0};
	UArray2_map_row_major(arr, check_row, &repeats);
	UArray2_map_col_major(arr, check_col, &repeats);
	check_square(arr, &repeats);
	UArray2_free(&arr);
	if(repeats[0] == 0){
		exit(0);
	}else{
		exit(1);
	}
}

//Makes sure that the file is a graymap pnm that has the correct denominator and dimensions..
Pnmrdr_T load_pnm(FILE *fp){
	Pnmrdr_T img = NULL;
	TRY
		img = Pnmrdr_new(fp);
	EXCEPT(Pnmrdr_Badformat)
		fclose(fp);
		Pnmrdr_free(&img);
		exit(1);
	ELSE
		fclose(fp);
		Pnmrdr_free(&img);
		exit(1);
	END_TRY;
	Pnmrdr_mapdata img_data = Pnmrdr_data(img);
	if(img_data.type != Pnmrdr_gray){
		fclose(fp);
		Pnmrdr_free(&img);
		exit(1);
	}
	if(img_data.denominator != 9){
		fclose(fp);
		Pnmrdr_free(&img);
		exit(1);
	}
	if(img_data.width != 9 || img_data.height != 9){
		fclose(fp);
		Pnmrdr_free(&img);
		exit(1);
	}
	return img;
}

UArray2_T pnm_to_uarray2(Pnmrdr_T img){
	Pnmrdr_mapdata img_data = Pnmrdr_data(img);
	UArray2_T arr = UArray2_new(img_data.width, img_data.height, sizeof(int));
	UArray2_map_row_major(arr, insert_pixels, &img);
	return arr;
}

void insert_pixels(int x, int y, void *val, void *cl){
	(void) x;
	(void) y;
	Pnmrdr_T *img = (Pnmrdr_T*) cl;
	*(int*) val = Pnmrdr_get(*img);
}

void check_row(int x, int y, void *val, void *cl){
	(void) y;
	int *digits = cl;
	int *curr = val;
	if(*curr < 0 || *curr > 9){
		digits[0] += 1;
	}else{
		digits[*curr] += 1;
	}
	if(x == 8){
		for(int i = 1; i < 10; ++i){
			if(digits[i] != 1){
				digits[0] += 1;
			}
			digits[i] = 0;
		}
	}
}

void check_col(int x, int y, void *val, void *cl){
	(void) x;
	int *digits = cl;
	int *curr = val;
	if(*curr < 0 || *curr > 9){
		digits[0] += 1;
	}else{
		digits[*curr] += 1;
	}
	if(y == 8){
		for(int i = 1; i < 10; ++i){
			if(digits[i] != 1){
				digits[0] += 1;
			}
			digits[i] = 0;
		}
	}
}
//checks 3x3 square for repeats
void check_square(UArray2_T arr, void *repeats){
	int *digits = repeats;
	for(int row = 0; row < 3; ++row){//each 3x3 square
		for(int col = 0; col < 3; ++col){
			for(int j = (row*3); j < (row*3 + 3); ++j){//each value in 3x3 square
				for(int i = (col*3); i < (col*3 + 3); ++i){
					int *curr = UArray2_at(arr, i, j);
					if(*curr < 0 || *curr > 9){
						digits[0] += 1;
					}else{
						digits[*curr] += 1;
					}
				}
			}
			//check for repeats
			for(int i = 1; i < 10; ++i){
				if(digits[i] != 1){
					digits[0] += 1;
				}
				digits[i] = 0;
			}
		}
	}
}
