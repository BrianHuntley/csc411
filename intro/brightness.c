/* Brian Huntley
 * CSC 411 - Intro - Part A
 */

#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>
#include <except.h>

void load_pnm(FILE *fp);
void calc_brightness(Pnmrdr_T img, Pnmrdr_mapdata img_data);

int main(int argc, char *argv[]){
	//Input file coming from argv
	if(argc == 2){
		FILE *fp = fopen(argv[1], "r");
		load_pnm(fp);
	}
	//Input coming from standard input
	else if(argc == 1){
		FILE *fp = stdin;
		load_pnm(fp);
	}
	else{
		fprintf(stderr, "%s: Too many arguments\n", argv[0]);
	}
}

//Makes sure the file is the correct format
void load_pnm(FILE *fp){
	TRY
		Pnmrdr_T img = Pnmrdr_new(fp);
		Pnmrdr_mapdata img_data = Pnmrdr_data(img);
		if(img_data.type != Pnmrdr_gray){
			fprintf(stderr, "File is not a graymap.\n");
		}
		calc_brightness(img, img_data);
		free(img);
	EXCEPT(Pnmrdr_Badformat)
		fprintf(stderr, "Bad file format.\n");
	ELSE
		fprintf(stderr, "File does not exist.\n");
		exit(1);
	END_TRY;
	fclose(fp);
}

//Calculates the average brightness and prints it.
void calc_brightness(Pnmrdr_T img, Pnmrdr_mapdata img_data){
	int pixel_count = img_data.width * img_data.height;
	int sum = 0;
	for(int i = 0; i < pixel_count; ++i){
		sum += Pnmrdr_get(img);
	}
	double average_brightness = sum / ((double)pixel_count * (double)img_data.denominator);
	printf("%1.3f\n", average_brightness);
}
