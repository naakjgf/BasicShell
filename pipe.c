#include "pipe.h"
#include "redirection.h"
#include "commands.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int handle_pipe(token *tokens) {
	int i = 0;
	while (tokens[i].type != PIPE && tokens[i].type != 0) {
		i++;
	}

	if (tokens[i].type == 0) {
		return 0;  //if there is no pipe
	}

	//splitting the tokens into two commands: before and after the pipe, so we can work on each individually
	token left_command[MAX_ARGS];
	token right_command[MAX_ARGS];
	int j;

	//filling the left_command
	for (j = 0; j < i; j++) {
		left_command[j] = tokens[j];
	}
	left_command[j].type = 0;  //marking the end of the commands for the left hand side

	//filling the right_commands
	int k = 0;
	i++;  //so we can skip the pipe token
	while (tokens[i].type != 0) {
		right_command[k] = tokens[i];
		k++;
		i++;
	}
	right_command[k].type = 0;  //marks the end of the command for right side

	//creating a pipe, fd[1] is for writing, fd[0] is for reading
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1) {
		perror("pipe");
		return 0;  //if creating the pipe failed
	}

	//forking into two seperate processes
	pid_t pid = fork();

	if (pid == 0) {
		// child process is intended to handle the left command (the one before the pipe)

		//redirecting stdout to the write end of the pipe
		close(pipe_fd[0]);  //close read end
		dup2(pipe_fd[1], STDOUT_FILENO); //replaces standard output with the write end of the pipe
		close(pipe_fd[1]); //close write end now that we are done w/ it, no more data coming in, at this point its output should end up
				   //in the write end of the pipe.

				   //executing the left command
		char *left_args[MAX_ARGS];
		for (int idx = 0; left_command[idx].type != 0; idx++) {
			left_args[idx] = left_command[idx].value;
		}
		left_args[j] = NULL;
		if (!execute_builtin(left_args) && !handle_redirection(left_command)) {
			//if not a built-in or redirection, call the execute function
			execute(left_command);
		}
		exit(EXIT_SUCCESS);

	} else if (pid > 0) {
		//parent process handles the right command (after the pipe)

		//redirect stdin from the read end of the pipe
		close(pipe_fd[1]);  //close write end
		dup2(pipe_fd[0], STDIN_FILENO); //replaces the standard input with the read end of the pipe
		close(pipe_fd[0]); //close read end, reading from the read end which includes data written by the child process
				   //essentially reading output from one command (the child which is the left side) and using it
				   //on this right side as the input for the parent

				   //executes the right command
		char *right_args[MAX_ARGS];
		for (int idx = 0; right_command[idx].type != 0; idx++) {
			right_args[idx] = right_command[idx].value;
		}
		right_args[j] = NULL;
		if (!execute_builtin(right_args) && !handle_redirection(right_command)) {
			// if not a built-in or redirection, call the execute function
			execute(right_command);
		}

		//waiting for the child to finish so they don't seperate
		int status;
		wait(&status);
	} else {
		perror("fork");
		return 0;  //the fork failed
	}

	return 1;  //if reaches this point, piping was successful
}
