#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>
#include "major2.h"

#define MAX_INPUT_LENGTH 512
#define MAX_ARGUMENTS 10
bool exitShell=false; // Flag to indicate whether to exit after commands are executed
int numAliases = 0;


void executeCommand(char* command) {
    char* args[MAX_ARGUMENTS];
    int numArgs = 0;
    char *output = strchr(command, '>');
    char *input = strchr(command, '<');

    if (output) {
        redirection(args, numArgs, command, 0);
    }
    
    else if (input) {
        redirection(args, numArgs, command, 1);
    }

    
    else {

    // Parse the command and arguments
    args[numArgs] = strtok(command, " ");
    while (args[numArgs] != NULL) {
        numArgs++;
        args[numArgs] = strtok(NULL, " ");
    }

    if (numArgs > 0) {
        // Built-in cd command
        if (strncmp(args[0], "cd", 10) == 0) {
            cd_builtin(args, numArgs);
        }

        // Built-in exit command
        else if (strncmp(args[0], "exit", 10) == 0) {
            exitShell=true; 
        }

        // Alias command
         else if (strncmp(args[0], "alias", 10) == 0) {
            if ((numArgs==3)&&(strcmp(args[1], "-r") == 0)) {
                removeAlias(args[2], &numAliases);
            }
            else if ((numArgs==2)&&(strcmp(args[1], "-c") == 0)) {
                removeAllAliases(&numAliases);
            }
            else{
            alias(args, numArgs, &numAliases);
            }

            
        }

        // Check for alias commands, execute if found
        else if (findAlias(args[0], numAliases)!=NULL) {
            executeAlias(args[0], &numAliases);
        }


        // External commands from user input
        else {
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
}

void runShell() {
    char input[MAX_INPUT_LENGTH];
    char* commands[MAX_ARGUMENTS];
    int numCommands;
    

    while (1) {
        printf("Shell> ");
        fgets(input, sizeof(input), stdin);

        // Remove the trailing newline character
        input[strcspn(input, "\r\n")] = '\0';

        // Parse the commands separated by semicolons
        numCommands = 0;
        commands[numCommands] = strtok(input, ";");
        while (commands[numCommands] != NULL) {
            numCommands++;
            commands[numCommands] = strtok(NULL, ";");
        }

        if (numCommands > 0) {
            for (int i = 0; i < numCommands; i++) {
                if (strlen(commands[i]) > 0) {
                    executeCommand(commands[i]);
                }
            }
            if (exitShell==true){
                exit(EXIT_SUCCESS);
            }
        }
    }
}

void runBatchMode(char* batchFileName) {
    // Open batch file to read commands
    FILE* batchFile = fopen(batchFileName, "r");
    if (batchFile == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char input[MAX_INPUT_LENGTH];
    char* commands[MAX_ARGUMENTS];
    int numCommands;

    while (fgets(input, sizeof(input), batchFile) != NULL) {
        // Remove the trailing newline character
        input[strcspn(input, "\r\n")] = '\0';

        printf("Shell> %s\n", input);
        
        // Parse the commands separated by semicolons
        numCommands = 0;
        commands[numCommands] = strtok(input, ";");
        while (commands[numCommands] != NULL) {
            numCommands++;
            commands[numCommands] = strtok(NULL, ";");
        }

        if (numCommands > 0) {
            for (int i = 0; i < numCommands; i++) {
                if (strlen(commands[i]) > 0) {
                    executeCommand(commands[i]);
                }
            }
        }
    }

    fclose(batchFile);
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        // Interactive mode
        runShell();
    } else if (argc == 2) {
        // Batch mode
        runBatchMode(argv[1]);
    } else {
        printf("Usage: %s [batch_file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}