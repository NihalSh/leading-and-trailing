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
	printf("\nLeading\n");
	for (i = 0; i < 256; i++) {
		if (variables[i]) {
			printf("%c => { ", i);
			//iterate over the production for a given symbol
			for (j = 0; j < productions[i].number; j++) {
				int loopOnVariable = i;
				char firstVariable = '\0';
				char firstTerminal = '\0';
				char currSymbol = '\0';
				while (loopOnVariable != '\0') {
					firstVariable = '\0';
					for (k = 0; k < productions[loopOnVariable].list[j].number; k++) {
						currSymbol = productions[loopOnVariable].list[j].c[k];
						if (terminals[currSymbol]) {
							firstTerminal = currSymbol;
							printf("%c ", firstTerminal);
							break;
						} else if (variables[currSymbol]) {
							if ((firstVariable == '\0')) {
								firstVariable = currSymbol;
							}
						} else if (currSymbol == '\0') {
							//null symbol
							;
						}else {
							printf("error: no idea what happended, character = %c\n", currSymbol);
						}
					}
					loopOnVariable = firstVariable;
				}
			}
			printf("}\n");
		}
	}
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