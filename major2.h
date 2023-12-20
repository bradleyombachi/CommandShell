#ifndef MAJOR2_H
#define MAJOR2_H

#endif

struct Alias {
    char name[50];
    char command[50];
};

struct Alias aliases[30];


// Function declarations
int cd_builtin(char **args, int numArgs);
int path();
int redirection(char **args, int numArgs, char* command, int flag);
void alias(char **args, int numArgs, int *numAliases);
void addAlias(char *name, char *command, int *numAliases);
char *findAlias(char *name, int numAliases);
void printAliases(int numAliases);
void executeAlias(char *name, int *numAliases);
void removeAlias(char *name, int *numAliases);
void removeAllAliases(int *numAliases); 