#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "execute.h"
#include "tokens.h"
#include "commands.h"
#include "redirection.h"
#include "pipe.h"

int main(int argc, char **argv) {
	char input[MAX_INPUT_LENGTH];
	token *tokens;
	printf("Welcome to mini-shell.\n");

	while (1) {
		printf("shell $ ");
		if (fgets(input, sizeof(input), stdin) == NULL) {
			printf("Bye bye.");
			break;
		}

		input[strcspn(input, "\n")] = '\0'; // we are trimming the input to remove the newline character
		tokens = tokenize(input);

		if (tokens[0].type == 0) {
			// empty line
			free_tokens(tokens);
			continue;
		}

		//current tokens of command being processed.
		token current_command[MAX_ARGS];
		int current_index = 0;

		for (int i = 0; tokens[i].type != 0; i++) {
			if (tokens[i].type == SEQUENCE || tokens[i + 1].type == 0) {
				//copying the token we are currently on into current_command's list of tokens
				current_command[current_index] = tokens[i];

				//if it's the last token and not a sequence token (;), increment the index, essentially skipping unnecessary things
				if (tokens[i + 1].type == 0 && tokens[i].type != SEQUENCE) {
					current_index++;
				}

				//marks the end of the current_command list with a null value, representing end of a string
				current_command[current_index].type = 0;
				current_command[current_index].value = NULL;

				//handling various types of commands, leaving the shell if user prompts 'exit'
				if (strcmp(current_command[0].value, "exit") == 0) {
					printf("Bye bye.");
					free_tokens(tokens);
					return 0;
				}

				//changing the token into a char ** because execute_builtin() only takes chars, same as I did in pipe.c
				char *command_strings[MAX_ARGS];
				int cmd_idx = 0;
				while (current_command[cmd_idx].type != 0) {
					command_strings[cmd_idx] = current_command[cmd_idx].value;
					cmd_idx++;
				}
				command_strings[cmd_idx] = NULL;  //NULL is the end of the array

				if (!execute_builtin(command_strings)) { //built-in commands
					if (!handle_redirection(current_command)) { //handling redirection
						if (!handle_pipe(current_command)) { //handling piping
							execute(current_command); //normal command execution
						}
					}
				}

				current_index = 0;
			} else {
				//if it isn't a sequence token, just copy to the current_command list
				current_command[current_index] = tokens[i];
				current_index++;
			}
		}

		free_tokens(tokens);
	}
	return 0;
}
