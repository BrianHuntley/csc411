/* Brian Huntley
 * CSC 411 - Intro - Part B
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <table.h>
#include <list.h>
#include <atom.h>
#include <except.h>

Table_T make_table();
void print_table(Table_T table);

int main(int argc, char *argv[]){
	if(argc > 1){
		fprintf(stderr, "%s: Too many args.\n", argv[0]);
	}
	Table_T table = make_table();
	print_table(table);
	Table_free(&table);
}

//Creates a table containing the fingerprints and names from stdin
Table_T make_table(){
	Table_T table = Table_new(0, NULL, NULL);

	int input_size = 1000;
	char* input = malloc(input_size);

	while(fgets(input, input_size, stdin) != NULL){
		TRY
			//separate fingerprint from name
			char *fingerprint = strtok(input, " ");
			char *name = strtok(NULL, "\n");
			//add value to table
			List_T name_list = NULL;
			if(Table_get(table, Atom_string(fingerprint)) == NULL){
				name_list = List_list((char *) Atom_string(name), NULL);
			}else{
				name_list = List_push(Table_get(table, Atom_string(fingerprint)), (char *) Atom_string(name));
			}
			Table_put(table, Atom_string(fingerprint), name_list);
		ELSE
			fprintf(stderr, "Input must be in the format: Key - Whitespace - Name - Newline\n");
		END_TRY;
	}
	free(input);
	return table;
}

//Print the filled out finger group table
void print_table(Table_T table){
	char **table_array = (char **) Table_toArray(table, NULL);
	for(int i = 1; i < (2 * Table_length(table)); i += 2){
		List_T values = (List_T) table_array[i];
		if(List_length(values) > 1){
			char **names = (char **) List_toArray(values, NULL);
			for(int j = 0; j < List_length(values); ++j){
				printf("%s\n", names[j]);
			}
				printf("\n");
			free(names);
		}
		List_free(&values);
	}
	free(table_array);
}
