#include "redirection.h"
#include <fcntl.h> //for the open function
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

extern void execute(token *tokens); //making use of execute from execute.c

int handle_redirection(token *tokens) {
    int i = 0;
    token command[MAX_ARGS];
    char *filename = NULL;

    while (tokens[i].type != 0) {
        if (tokens[i].type == REDIRECT_IN || tokens[i].type == REDIRECT_OUT) {
            // copy tokens before the redirection symbol to the command
            for (int j = 0; j < i; j++) {
                command[j] = tokens[j];
            }
            command[i].type = 0;  //mark the end of the command

            filename = tokens[i + 1].value;  //filename is the token after the redirection symbol

            if (tokens[i].type == REDIRECT_IN) {
                execute_with_input_redirection(command, filename);
                return 1;
            } else if (tokens[i].type == REDIRECT_OUT) {
                execute_with_output_redirection(command, filename);
                return 1;
            }
        }
        i++;
    }
    return 0;  //no redirection found
}


void execute_with_input_redirection(token *tokens, char *file) {
    int fd = open(file, O_RDONLY); //opening a file for reading
    if (fd == -1) {
        perror("Error opening file for reading");
        return;
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        dup2(fd, 0); // redirect standard input (file descriptor 0) to come from file we have represented by fd
        close(fd); // closing file descriptor
        execute(tokens); //executing the command
        exit(0);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("Fork failed");
    }
}

void execute_with_output_redirection(token *tokens, char *file) {
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666); //opens file, either as write only, creates it if it doesn't exist, or clears out an existing file, 0666 supposedly
							     //allows anyone to read or write to a file, including all file groups.
    if (fd == -1) {
        perror("Error opening file for writing");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
	    dup2(fd, 1); //redirects stdout (file descriptor 1) to go to file
	    close(fd); //closes file descriptor
	    execute(tokens); //executes the command
	    exit(0);
    } else if (pid > 0) {
	    wait(NULL);
    } else {
	    perror("Fork failed");
    }
}

