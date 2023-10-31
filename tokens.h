#ifndef TOKENS_H
#define TOKENS_H

// define tokens
#define OPEN_PARENTHESES 1
#define CLOSE_PARENTHESES 2
#define REDIRECT_IN 3
#define REDIRECT_OUT 4
#define SEQUENCE 5
#define PIPE 6
#define QUOTE 7
#define WORD 8

#define MAX_INPUT_SIZE 255
#define MAX_TOKEN_SIZE 255

typedef struct {
  int type;
  char *value;
} token;

token *tokenize(const char *input);

#endif
