#ifndef REDIRECTION_H
#define REDIRECTION_H

#include "execute.h"
#include "tokens.h"

int handle_redirection(token *tokens);
void execute_with_input_redirection(token *tokens, char *file);
void execute_with_output_redirection(token *tokens, char *file);

#endif
