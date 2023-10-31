#ifndef EXECUTE_H
#define EXECUTE_H

#include "tokens.h"

#define MAX_INPUT_LENGTH 255
#define MAX_ARGS 32

//A function which helps us execute a command.
void execute(token *tokens);

#endif
