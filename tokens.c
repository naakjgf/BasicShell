#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int isToken(char c) {
	return c == '(' || c == ')' || c == '<' || c == '>' || c == ';' || c == '|';
}

int isWhitespace(char c) {
	return c == ' ' || c == '\t' || c == '\n';
}

/** Helper function to allow ease of copying strings **/
char* str_duplicate(const char *src) {
	size_t len = strlen(src); //geting length of source string
	char *dest = (char *)malloc(len + 1);  //allocating mem for the new string
	assert(dest != NULL);
	strcpy(dest, src);  //copying the string into a destination string
	return dest;
}

void free_tokens(token *tokens) {
    int i = 0;
    while (tokens[i].type != 0) {
        free(tokens[i].value); //freeing each string value in the token
        i++;
    }
    free(tokens); //frees the token array itself
}


// Represents a string as a series of tokens
token *tokenize(const char *input) {
	token *tokens = malloc(MAX_INPUT_SIZE * sizeof(token));
	int tokenCount = 0;
	int i = 0;
	int inside_quotes = 0; // a flag to tell us if we are in quotes or not

	// check all inputs until null
	while (input[i] != '\0') {
		while (isWhitespace(input[i]) && !inside_quotes) {
			// skip -> try to get a string    
			i++;
		}

		if (input[i] == '"') {
			i++;  // we skip the opening quote so it isn't included in token
			char tokenBuffer[MAX_TOKEN_SIZE];
			int j = 0;

			// capturing all the text within the quotes to make into one token
			while (input[i] != '\0' && input[i] != '"') {
				tokenBuffer[j] = input[i];
				i++;
				j++;
			}

			if (input[i] == '"') {
				i++;  // skipping the closing quote as well
			}

			tokenBuffer[j] = '\0';
			tokens[tokenCount].type = WORD;
			tokens[tokenCount].value = str_duplicate(tokenBuffer);
			tokenCount++;
			continue;
		}

		if (isToken(input[i])) {
			tokens[tokenCount].value = malloc(2);
			tokens[tokenCount].value[0] = input[i];
			tokens[tokenCount].value[1] = '\0';
			switch (input[i]) {
				case '(':
					tokens[tokenCount].type = OPEN_PARENTHESES;
					break;
				case ')':
					tokens[tokenCount].type = CLOSE_PARENTHESES;
					break;
				case '<':
					tokens[tokenCount].type = REDIRECT_IN;
					break;
				case '>':
					tokens[tokenCount].type = REDIRECT_OUT;
					break;
				case ';':
					tokens[tokenCount].type = SEQUENCE;
					break;
				case '|':
					tokens[tokenCount].type = PIPE;
					break;
			}
			tokenCount++;
			i++;
		} else {
			// not a token but can be double quotes
			char tokenBuffer[MAX_TOKEN_SIZE];
			int j = 0;
			while (input[i] != 0 && !isToken(input[i]) && !isWhitespace(input[i])) {
				tokenBuffer[j] = input[i];
				i++;
				j++;
			}
			tokenBuffer[j] = '\0';
			tokens[tokenCount].type = WORD;
			tokens[tokenCount].value = str_duplicate(tokenBuffer);
			tokenCount++;
		}
	}
	tokens[tokenCount].type = 0;
	tokens[tokenCount].value = NULL;
	return tokens;
}

