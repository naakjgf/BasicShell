#ifndef COMMANDS_H
#define COMMANDS_H

#include "execute.h"

#define MAX_INPUT_LENGTH 255

// Function to execute built-in commands. Returns 1 if command is a built-in, 0 otherwise.
int execute_builtin(char **args);

#endif
