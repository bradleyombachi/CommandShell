#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "major2.h"
// Built in cd command
int cd_builtin(char **args, int numArgs) {
  const char *home_dir = getenv("HOME");
    if (args[1] == NULL) {
        if (chdir(home_dir) != 0) {
        perror("chdir");
    }
    } 
    else if (numArgs>2) {
      fprintf(stderr, "cd: too many arguments\n");

    }
    else {
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    }
    return 1;
}
