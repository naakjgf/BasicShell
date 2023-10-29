#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 255
#define MAX_TOKEN_SIZE 255

// define tokens
#define OPEN_PARENTHESES 1
#define CLOSE_PARENTHESES 2
#define REDIRECT_IN 3
#define REDIRECT_OUT 4
#define SEQUENCE 5
#define PIPE 6
#define QUOTE 7
#define WORD 8

typedef struct {
  int type;
  char *value;
} token;

int isToken(char c) {
  return c == '(' || c == ')' || c == '<' || c == '>' || c == ';' || c == "|";
}

int isWhitespace(char c) {
  return c == ' ' || c == '\t' || c == '\n';
}

// Represents a string as a series of tokens
token *tokenize(const char *input) {
  token *tokens = malloc(MAX_INPUT_SIZE * sizeof(token));
  int tokenCount = 0;
  int i = 0;
  // check all inputs until null
  while (input[i] != '\0') {
    while (isWhitespace(input[i])) {
      // skip -> try to get a string    
      i++;
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
      char command[MAX_TOKEN_SIZE];
      int j = 0;
      while (input[i] != 0 && !isToken(input[i]) && !isWhitespace(input[i])) {
        command[j] = input[i];
	i++;
	j++;
      }
      command[j] = '\0';
      tokens[tokenCount].type = WORD;
      tokens[tokenCount].value = strdup(command);
      tokenCount++;
    }
  }
  tokens[tokenCount].type = 0;
  tokens[tokenCount].value = NULL;
  return tokens;
}


int main(int argc, char **argv) {
  char input[MAX_INPUT_SIZE];
  fgets(input, sizeof(input), stdin);		// gets keyboard input
  token *tokens = tokenize(input);
  // print tokens
  for (int i = 0; tokens[i].type != 0; i++) {
    printf("%s\n", tokens[i].value);
    free(tokens[i].value);
  }
  free(tokens);
  return 0;
}
