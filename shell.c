#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 255
#define MAX_ARGS 32

typedef struct {
  int type;
  char *value;
} token;

extern token *tokenize(const char *input); 	// call to tokenizer.c

void execute(token *tokens) {
  int status;
  pid_t pid = fork();
  if (pid == 0) {
    // in child
    char *args[MAX_ARGS];
    int i = 0;
    while (tokens[i].type != 0) {
      // get command and args
      args[i] = tokens[i].value;
      i++;
    }
    args[i] = NULL;
    execvp(args[0], args);
    // error if exec fails
    fprintf(stderr, "%s: Invalid command\n", args[0]);
    exit(1);
  } else if (pid > 0) {
    // in parent
    wait(&status);
  } else {
    fprintf(stderr, "Fork failed\n");
  }
}

int main(int argc, char **argv) {
  char input[MAX_INPUT_LENGTH];
  token *tokens;
  printf("Welcome to mini-shell.\n");
  // infinite loop unless exit
  while (1) {
    printf("shell $ ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
      // end
      printf("Bye bye.");
      break;
    }
    tokens = tokenize(input);
    if (tokens[0].type == 0) {
      // empty line
      free(tokens);
      continue;
    }
    if (strcmp(tokens[0].value, "exit") == 0) {
      printf("Bye bye.");
      free(tokens);
      break;
    }
    execute(tokens);
    free(tokens);
  }
  return 0;
}
