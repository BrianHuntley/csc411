/* Brian Huntley
 * CSC 411 - Binary Bomb
 */

#include "list.h"
#include <stdlib.h>
#include <stdio.h>

static char *PASS = "He is evil and fits easily into most overhead storage bins.";

static char *CIPHER = "maduiersnfotvbyl";

extern void explode_bomb();

extern int strings_not_equal(char *input, char *key);

extern int* read_six_numbers(char *input);

int func4(int search, int lo, int hi){
	int mid = (lo + hi)/2;
	if(mid == search){
		return 0;
	}else if(mid > search){
		return func4(search, lo, mid) + 1;
	}else{
		return func4(search, mid, hi) + 1;
	}
}

void phase1(char *input){
	if(strings_not_equal(input, PASS)){
		explode_bomb();
	}
	return;
}

void phase2(char *input){
	int* inputs = read_six_numbers(input);
	for(int i = 0; i < 6; ++i){
		int ans = (i + 1) + inputs[i];
		if(inputs[i + 1] != ans){
			explode_bomb();
		}
	}
	return;
}

void phase3(char *input){
	int tmp1;
	int tmp2;

	int is_valid = sscanf(input, "%d %d", &tmp1, &tmp2);

	if(is_valid != 2){
		explode_bomb();
	}

	int pass = 0;

	//The lack of break statements is intentional.
	switch (tmp1){

		case 0:
			pass -= 345;
		case 1:
			pass += 717;
		case 2:
			pass -= 350;
		case 3:
			pass += 350;
		case 4:
			pass -= 350;
		case 5:
			pass += 350;
		case 6:
			pass -= 350;
	}
	if(pass != tmp2){
		explode_bomb();
	}
	return;
}

void phase4(char *input){
	int tmp1;
	int tmp2;

	int is_valid = sscanf(input, "%d %d", &tmp1, &tmp2);

	if(is_valid != 2){
		explode_bomb();
	}

	if(tmp2 != 1){
		explode_bomb();
	}

	if(func4(tmp1, 0, 14) != 1){
		explode_bomb;
	}

	return;
}

void phase5(char *input){
	char pass[6];
	for(int i = 0; i < 6; ++i){
		int idx = input[i] & 0xf;
		pass[i] = CIPHER[idx];
	}
	if(strings_not_equal(pass, "flames")){
		explode_bomb();
	}

	return;
}

void phase6(char *input){
	int *inputs = read_six_numbers(input);
	for(int i = 0; i < 6; ++i){
		for(int j = i; j < 6; ++j){
			if(inputs[i] == inputs[j]){
				explode_bomb();
			}
		}
	}

	int vals[6] = {517, 154, 518, 74, 750, 313};
	List_T input_list = NULL;

	for(int i = 0; i < 6; ++i){
		int idx = inputs[i];
		List_push(input_list, vals[idx]);
	}
	int **ascending_order = (int **) Table_toArray(input_list, NULL);
	for(int i = 0; i < 5; ++i){
		if(ascending_order[i] > ascending_order[i+1]){
			explode_bomb();
		}
	}
	return;
}
