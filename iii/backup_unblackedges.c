#include <stdlib.h>
#include <stdio.h>

#include "bit2.h"
#include "pnmrdr.h"
#include "except.h"

Pnmrdr_T load_pnm(FILE *fp);
Bit2_T pnm_to_bit2(Pnmrdr_T img);
void print_bit2(int x, int y, int val, void *cl);
Bit2_T unblack_edges(Bit2_T bitmap);
int has_neightbor(Bit2_T bitmap, int x, int y);

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
		fprintf(stderr, "Can only take one argument\n");
                exit(1);
        }
        Pnmrdr_T img = load_pnm(fp);
	Bit2_T bitmap = pnm_to_bit2(img);
	fclose(fp);
	Pnmrdr_free(&img);
	Bit2_T no_edges = unblack_edges(bitmap);
	int width = Bit2_width(no_edges);
	printf("P1 \n%d %u \n", width, Bit2_height(no_edges));
	Bit2_map_row_major(no_edges, print_bit2, &width);
	Bit2_free(&bitmap);
	Bit2_free(&no_edges);
}

//Makes sure that the file is a graymap pnm that has the correct denominator and dimensions..
Pnmrdr_T load_pnm(FILE *fp){
        Pnmrdr_T img = NULL;
        TRY
                img = Pnmrdr_new(fp);
        EXCEPT(Pnmrdr_Badformat)
		fprintf(stderr, "Bad file format\n");
                fclose(fp);
                Pnmrdr_free(&img);
                exit(1);
        ELSE
		fprintf(stderr, "File does not exist\n");
                fclose(fp);
                Pnmrdr_free(&img);
                exit(1);
        END_TRY;
        Pnmrdr_mapdata img_data = Pnmrdr_data(img);
        if(img_data.type != Pnmrdr_bit){
		fprintf(stderr, "File is not a pbm\n");
                fclose(fp);
                Pnmrdr_free(&img);
                exit(1);
        }
        return img;
}

Bit2_T pnm_to_bit2(Pnmrdr_T img){
	Pnmrdr_mapdata img_data = Pnmrdr_data(img);
	Bit2_T tmp = Bit2_new(img_data.width, img_data.height);
	for(unsigned int i = 0; i < img_data.height; ++i){
		for(unsigned int j = 0; j < img_data.width; ++j){
			int bit = Pnmrdr_get(img);
			Bit2_put(tmp, j, i, bit);
		}
	}
	return tmp;
}

void print_bit2(int x, int y, int val, void *cl){
	(void) y;
	printf("%d ", val);
	int *width = (int*) cl;
	if(x == *width-1){
		printf("\n");
	}
}

Bit2_T unblack_edges(Bit2_T bitmap){
	int width = Bit2_width(bitmap);
	int height = Bit2_height(bitmap);
	Bit2_T no_edges = Bit2_new(width, height);
	//make a copy of the original
	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			int tmp = Bit2_get(bitmap, j, i);
			Bit2_put(no_edges, j, i, tmp);
		}
	}
	//Top checking down
	for(int i = 0; i < width; ++i){
		if(Bit2_get(bitmap, i, 0)){
			Bit2_put(no_edges, i, 0, 0);
			for(int j = 1; j < height; ++j){
				if(Bit2_get(bitmap, i, j)){
					Bit2_put(no_edges, i, j, 0);
				}else{
					break;
				}
			}
		}
	}
	//Right checking left
	for(int i = 0; i < height; ++i){
		if(Bit2_get(bitmap, width - 1, i)){
			Bit2_put(no_edges, width - 1, i, 0);
			for(int j = width - 2; j >= 0; --j){
				if(Bit2_get(bitmap, j, i)){
					Bit2_put(no_edges, j, i, 0);
				}else{
					break;
				}
			}
		}
	}
	//Bottom checking up
	for(int i = 0; i < width; ++i){
		if(Bit2_get(bitmap, i, height - 1)){
			Bit2_put(no_edges, i, height - 1, 0);
			for(int j = height - 2; j >= 0; --j){
				if(Bit2_get(bitmap, i, j)){
					Bit2_put(no_edges, i, j, 0);
				}else{
					break;
				}
			}
		}
	}
	//Left checking right
	for(int i = 0; i < height; ++i){
		if(Bit2_get(bitmap, 0, i)){
			Bit2_put(no_edges, 0, i, 0);
			for(int j = 1; j < width; ++j){
				if(Bit2_get(bitmap, j, i)){
					Bit2_put(no_edges, j, i, 0);
				}else{
					break;
				}
			}
		}
	}
	return no_edges;
}
int has_neighbor(Bit2_T bitmap, int x, int y){
	if(x > 0){
		int left_neighbor = Bit2_get(bitmap, x - 1, y);
		if(left_neighbor){
			return 1;
		}
	}
	if(x < Bit2_width(bitmap) - 1){
		int right_neighbor = Bit2_get(bitmap, x + 1, y);
		if(right_neighbor){
			return 1;
		}
	}
	if(y > 0){
		int top_neighbor = Bit2_get(bitmap, x, y - 1);
		if(top_neighbor){
			return 1;
		}
	}
	if(y < Bit2_height(bitmap) - 1){
		int bottom_neighbor = Bit2_get(bitmap, x, y + 1);
		if(bottom_neighbor){
			return 1;
		}
	}
	return 0;
}

