#include "execute.h"
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute(token *tokens) {
	char *args[MAX_ARGS];
	int i = 0;
	while (tokens[i].type != 0) {
		args[i] = tokens[i].value;
		i++;
	}
	args[i] = NULL;

	if (execute_builtin(args)) { //executes the built in command if it is one of the few we have
		return;
	}

	int status;
	pid_t pid = fork();
	if (pid == 0) {
		execvp(args[0], args);
		fprintf(stderr, "%s: command not found\n", args[0]);
		exit(1);
	} else if (pid > 0) {
		wait(&status);
	} else {
		fprintf(stderr, "Fork failed\n");
	}
}
