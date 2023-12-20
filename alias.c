#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include "major2.h"

#define MAX_ALIASES 10
#define MAX_ALIAS_LENGTH 50
#define MAX_COMMAND_LENGTH 256


// Function to add an alias
void addAlias(char *name, char *command, int *numAliases) {
    if (*numAliases < MAX_ALIASES) {
        strcpy(aliases[*numAliases].name, name);
        strcpy(aliases[*numAliases].command, command);
        (*numAliases)++;
    } else {
        fprintf(stderr, "Error: Maximum number of aliases reached.\n");
    }
}

//Remove an alias
void removeAlias(char *name, int *numAliases) {
    for (int i = 0; i < *numAliases; i++) {
        if (strcmp(aliases[i].name, name) == 0) {
            // Shift elements to remove the alias
            for (int j = i; j < *numAliases - 1; j++) {
                strcpy(aliases[j].name, aliases[j + 1].name);
                strcpy(aliases[j].command, aliases[j + 1].command);
            }
            (*numAliases)--;
            printf("Alias removed: %s\n", name);
            return;
        }
    }
    printf("Alias not found: %s\n", name);
}

//Remove all aliases
void removeAllAliases(int *numAliases) {
    *numAliases = 0;
    printf("All aliases removed.\n");
}

// Function to find an alias by name
char *findAlias(char *name, int numAliases) {
    for (int i = 0; i < numAliases; i++) {
        if (strcmp(aliases[i].name, name) == 0) {
            return aliases[i].command;
        }
    }
    return NULL;
}

// Function to print all defined aliases
void printAliases(int numAliases) {
    printf("Defined aliases:\n");
    for (int i = 0; i < numAliases; i++) {
        printf("%s=%s\n", aliases[i].name, aliases[i].command);
    }
}

// Process and execute a command
void executeAlias(char *name, int *numAliases) {
    char *aliasCommand = findAlias(name, *numAliases);
    if (aliasCommand != NULL) {
        char commandCopy[MAX_COMMAND_LENGTH];  // Create a copy
        strcpy(commandCopy, aliasCommand);

        char *args[50];
        int numArgs = 0;

        args[numArgs] = strtok(commandCopy, " ");
        while (args[numArgs] != NULL) {
            numArgs++;
            args[numArgs] = strtok(NULL, " ");
        }

        if (numArgs > 0) {
            pid_t pid = fork();

            if (pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                // Child process
                if (execvp(args[0], args) < 0) {
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
            } else {
                // Parent process
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }
}


void alias(char **args, int numArgs, int *numAliases) {

    
    // Check if no arguments are provided and print aliases
    if (numArgs == 1) {
        printAliases(*numAliases);
        return;
    }

    // Check if the format is incorrect
    if (strchr(args[1], '=') == NULL) {
        fprintf(stderr, "Error: Incorrect format (alias alias_name='command')\n");
        return;
    }

    char *aliasName = strtok(args[1], "=");

    // Check if aliasName is NULL after strtok
    if (aliasName == NULL) {
        fprintf(stderr, "Error: Incorrect format (alias alias_name='command')\n");
        return;
    }

    char fullCommand[MAX_COMMAND_LENGTH] = ""; // Initialize as an empty string
    char modified[MAX_COMMAND_LENGTH];

    char *aliasCmd = strtok(NULL, " ");
    strcpy(modified, aliasCmd + 1);

    if (numArgs == 2) {

        size_t length = strlen(modified);
        if (length > 0) {
            modified[length - 1] = '\0';
        } 
    }

    strcat(fullCommand, modified);

    // Concatenate the rest of the arguments
    if (numArgs > 2) {
        for (int i = 2; i < numArgs; i++) {
            size_t length = strlen(args[i]);
            if (i == numArgs - 1) {
                args[i][length - 1] = '\0';
            }
            strcat(fullCommand, " ");
            strcat(fullCommand, args[i]);
        }
    }

    // Add the alias to the list
    addAlias(aliasName, fullCommand, numAliases);
}
