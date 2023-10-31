#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int execute_builtin(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "cd: argument required\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        }
        return 1;
    }

    if (strcmp(args[0], "source") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "source: argument required\n");
            return 1;
        }

        FILE *fp = fopen(args[1], "r");
        if (fp == NULL) {
            perror("source");
            return 1;
        }

        char line[MAX_INPUT_LENGTH];
        while (fgets(line, sizeof(line), fp) != NULL) {
            token *tokens = tokenize(line);
            execute(tokens);
            free(tokens);
        }

        fclose(fp);
        return 1;
    }

    if (strcmp(args[0], "help") == 0) {
        printf("cd - Changes your directory\n");
        printf("source - Executes a script\n");
        printf("prev - Prints the previous command and executes it\n");
        printf("help - Explains all the built-in commands\n");
        printf("exit - Exit the shell\n");
        return 1;
    }

    return 0;  //returning zero if it ends up not being a built in command
}

