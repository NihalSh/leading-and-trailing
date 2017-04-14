#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int number;
	char * c;
} Body;

typedef struct {
	int number;
	Body * list;
} Production;

int main()
{
	int variables[256] = {0};
	int terminals[256] = {0};
	Production productions[256];
	getInput(variables, terminals, productions);
/*
	int i;
	int j;
	for (i = 0; i < 256; i++) {
		if (variables[i]) {
			printf("%c ", i);
		}
	}
	printf("\n");
	for (i = 0; i < 256; i++) {
		if (terminals[i]) {
			printf("%c ", i);
		}
	}
	printf("\n");
	for (i = 0; i < 256; i++) {
		if (productions[i].number) {
			for (j = 0; j < productions[i].number; j++) {
				printf("%c -> %s\n", i, productions[i].list[j].c);
			}

		}
	}
*/
	computeLeading(variables, terminals, productions);
	return 0;
}

int computeLeading(int * variables, int * terminals, Production productions[256])
{
	int i;
	int j;
	int k;
	int ** leading = (int **) malloc(256 * sizeof(int *));
	for (i = 0; i < 256; i++) {
		if (variables[i]) {
			leading[i] = (int *) malloc(256 * sizeof(int));
		}
		else {
			leading[i] = NULL;
		}
	}
	for (i = 0; i < 256; i++) {
		if (variables[i]) {
			//iterate over the production for a given symbol
			for (j = 0; j < productions[i].number; j++) {
				char currSymbol = '\0';
				int notNullableEncountered = 0;
				for (k = 0; k < productions[i].list[j].number; k++) {
					currSymbol = productions[i].list[j].c[k];
					if (terminals[currSymbol]) {
						leading[i][currSymbol] = 1;
						break;
					} else if (variables[currSymbol]) {
						if (notNullableEncountered) {
							;
						} else {
							if (isNullable(productions[currSymbol])) {

							} else {
								notNullableEncountered = 1;
							}
							leading[i][currSymbol] = 1;//add to leading variables till the not nullable variable is encountered
						}
					} else if (currSymbol == '\0') {
						//null symbol
						;
					}else {
						printf("error: no idea what happended, character = %c\n", currSymbol);
					}
				}
			}
		}
	}
	expand(variables, leading);
	printf("\nLeading\n");
	display(variables, leading);
}

int expand(int * variables, int ** leadingOrTrailing)
{
	int i;
	int j;
	int k;
	for (i = 0; i < 256; i++) {
		if (variables[i]) {
			int boolean = 1;
			int z = 3;
			while (boolean) {
				boolean = 0;
				for (j = 0; j < 256; j++) {
					if ((leadingOrTrailing[i][j] == 1) && variables[j]) {
						boolean = 1;
						//copy leadingOrTrailing of variable 'j' into 'i'
						for (k = 0; k < 256; k++) {
							if ((leadingOrTrailing[j][k] == 1) && (leadingOrTrailing[i][k] == 0)) {
								//printf("Expanding: %c, Expanding with: %c, %c\n", i, j, k);
								leadingOrTrailing[i][k] = 1;
							}
						}
						leadingOrTrailing[i][j] = -1;

					}
				}
			}
		}
	}
}

int display(int * variables, int ** leading)
{
	int i;
	int j;
	for (i = 0; i < 256; i++) {
		if (variables[i]) {
			printf("(%c) = { ", i);
			for (j = 0; j < 256; j++) {
				if (leading[i][j] == 1) {
					printf("%c ", j);
				}
			}
			printf("}\n");
		}
	}
	return 0;
}

int isNullable(Production production) {
	int i;
	for (i = 0; i < production.number; i++) {
		if (production.list[i].number && (production.list[i].c[0] == '\0')) {
			return 1;
		}
	}
	return 0;
}

int getInput(int * variables, int * terminals, Production productions[256])
{
	int i;
	int j;
	int c;
	int numVariables;
	int numTerminals;
	int numProductions;
	char head;
	char body[50];//just an assumption for input length
	printf("Variables: ");
	scanf(" %d", &numVariables);
	for (i = 0; i < numVariables; i++) {
		scanf(" %c", &c);
		variables[c] = 1;
	}
	printf("Terminals: ");
	scanf(" %d", &numTerminals);
	for (i = 0; i < numTerminals; i++) {
		scanf(" %c", &c);
		terminals[c] = 1;
	}
	/*initialize production array*/
	for (i = 0; i < 256; i++) {
		productions[i].number = 0;
		productions[i].list= NULL;
	}
	printf("Number of Productions: ");
	scanf(" %d", &numProductions);
	for (i = 0; i < numProductions; i++) {
		scanf(" %c %s", &head, body);
		if (variables[head]) {
			productions[head].number++;
			productions[head].list = (Body *) realloc(productions[head].list, productions[head].number * sizeof(Body));
			productions[head].list[productions[head].number - 1].number = 0;
			productions[head].list[productions[head].number - 1].c = NULL;
			for (j = 0; j < strlen(body); j++) {
				productions[head].list[productions[head].number - 1].number++; 
				productions[head].list[productions[head].number - 1].c = (char *) realloc(productions[head].list[productions[head].number - 1].c, productions[head].list[productions[head].number - 1].number * sizeof(char));
				if (variables[body[j]] || terminals[body[j]]) {
					productions[head].list[productions[head].number - 1].c[productions[head].list[productions[head].number - 1].number - 1]  = body[j];
				} else {
					//this character is treated as a null character (which is neither in variables nor terminals)
					productions[head].list[productions[head].number - 1].c[productions[head].list[productions[head].number - 1].number - 1]  = '\0';
				}
			}
		}
	}
	return 0;
}