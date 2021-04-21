/* Brian Huntley
 * CSC 411 - Universal Machine
 */

#include <stdlib.h>
#include <stdio.h>

#include "seg.h"
#include "load.h"
#include "execute.h"

int main(int argc, char *argv[]){

	if(argc != 2){
		fprintf(stderr, "Invalid arguments.\n");
	}

	FILE *fp = fopen(argv[1], "r");
	if(fp == NULL){
		fprintf(stderr, "Bad file.\n");
		fclose(fp);
		exit(1);
	}

	//load program
	Seg_T prog = read_program(fp);
	fclose(fp);

	//run program
	execute_program(prog);
	Seg_free(&prog);

	return 0;
}
