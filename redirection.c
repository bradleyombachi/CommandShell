#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include "major2.h"

int redirection(char **args, int numArgs, char* command, int flag){
  //output redirection
  if (flag==0) {
    char *cmd = strtok(command, ">");
        char *writeFile = strtok(NULL, " \n");

        //Parse command
        args[numArgs] = strtok(cmd, " ");
        while (args[numArgs] != NULL) {
            numArgs++;
            args[numArgs] = strtok(NULL, " ");
        }
        
    if (cmd != NULL && writeFile != NULL) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            int fd = open(writeFile, O_CREAT | O_TRUNC | O_WRONLY, 0600);
            if (fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }

            // Redirect stdout to the file
            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("dup2");
                exit(EXIT_FAILURE);
            }

            close(fd); // Close the original file descriptor

            // Execute the command
            if (execvp(args[0], args) < 0) {
                perror("execlp");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        }
        
    }
  }

  // input redirection
  else {
    strtok(command, "<");
    char *fileName = strtok(NULL, " \n");
    char input[100];
    FILE* inputFile = fopen(fileName, "r");
    
    if (fgets(input, sizeof(input), inputFile) != NULL){
      args[numArgs] = strtok(input, " ");
      while (args[numArgs] != NULL) {
          numArgs++;
          args[numArgs] = strtok(NULL, " ");
      }
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
  
  
  return 1;
}