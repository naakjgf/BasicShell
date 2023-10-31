#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"

int main(int argc, char **argv) {
	char input[MAX_INPUT_SIZE];
	fgets(input, sizeof(input), stdin);	// gets keyboard input
	token *tokens = tokenize(input);
	// print tokens
	for (int i = 0; tokens[i].type != 0; i++) {
		printf("%s\n", tokens[i].value);
		free(tokens[i].value);
	}
	free(tokens);
	return 0;
}
