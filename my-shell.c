/* my-shell.c: a simple shell programs that demonstrates my knowledge of the fork, execvp, and wait system calls.
 * Compile: gcc -o my-shell my-shell.c
 * Run: ./my-shell
 * Description: This program implements a basic shell that allows users to execute commands 
 *  in a Unix-like environment. It supports parallel command execution, directory navigation using `cd`, 
 *  and exiting the shell with the "exit" or "quit" commands. The shell uses `fork()` to create child 
 *  processes for command execution, `execvp()` to run commands, and `waitpid()` to synchronize processes.
 *  Users can input multiple commands separated by `#` to run them in parallel, 
 *  with the shell waiting only for the last command to finish.
 * 
 * Links:
 * - `fork()` documentation: https://man7.org/linux/man-pages/man2/fork.2.html
 * - `execvp()` documentation: https://man7.org/linux/man-pages/man3/execvp.3.html
 * - `waitpid()` documentation: https://man7.org/linux/man-pages/man2/waitpid.2.html
 * - `chdir()` documentation: https://man7.org/linux/man-pages/man2/chdir.2.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char ***getCommands();

/* The readInput function prompts the user to enter a line of text.
 * It dynamically allocates memory for the input string and returns a
 * pointer to the first character of the string.
 * The caller is responsible for freeing the allocated memory after use.
*/
char * readInput();

/* The parse function accepts a string (pointer to char) and a character (pointer to a char)
 * to act as a delimiter. When string is NULL or empty (""), 
 * it returns a list of one item (pointer) and the value of the pointer is NULL. 
 * Otherwise, it returns a list of pointers pointing to the tokens and the last pointer must be NULL. 
*/
char ** parse(char *string, const char *delimiter);

/* The isExitCommand function checks if the first token in tokenList is "exit" or "quit"
 * and whether there are no additional tokens in the list. If both conditions are met,
 * it returns 1 (true), indicating the user wants to exit. Otherwise, it returns 0 (false).
*/
int isExitCommand(char **tokenList);

/* The isCDCommand function checks if the first token in tokenList is "cd"
 * and whether there are no additional tokens in the list. Then uses chdir
 * to change the directory appropriately.
*/
int isCDCommand (char **tokenList);

/* The executeCommand function forks the process and runs the command specified by
 * the first token in tokenList using execvp. If execvp fails, it prints an error
 * message. The arent process waits for the child process to finish before proceeding.
*/
void executeCommands(char ***commandList);

/* The freeListOfLists function accepts a list of lists (tokenLists) each having tokens.
 * tokenLists would never be NULL, but they could be empty (the only item is a NULL pointer in the
 * lists). It frees the spaces used by all tokens and the pointer lists.
*/
void freeListOfLists(char *** tokenList);

/* The freeList function accepts a list (tokenList) of tokens with a NULL pointer as the last item.
 * tokenList would never be NULL, but it could be empty (the only item is a NULL pointer in the
 * list). It frees the spaces used by all tokens and the pointer list.
*/
void freeList(char ** tokenList);

void printListOfLists(char ***listOfLists);


int main() {
    while (1) {
        char ***commandList = getCommands();
        // printListOfLists(commandList);

       if (commandList[0][0] != NULL) {
            if (isExitCommand(commandList[0])) {
                freeListOfLists(commandList);
                break;
            }
            if (isCDCommand(commandList[0])) {
                chdir(commandList[0][1]);
            } else {
                executeCommands(commandList);
            }
        }
        freeListOfLists(commandList);
    }
    return 0;
}

char ***getCommands() {
    char *input = readInput();
    char **rawCommandList = parse(input, "#");
    free(input);

    size_t numCommands = 0;
    while (rawCommandList[numCommands] != NULL) {
        numCommands++;
    }

    char ***commandList = malloc((numCommands + 1) * sizeof(char **)); // +1 for NULL

    for (size_t i = 0; i < numCommands; i++) {
        commandList[i] = parse(rawCommandList[i], " \t\n");
    }
    commandList[numCommands] = NULL;

    freeList(rawCommandList);
    return commandList;
}

char * readInput() {
    char *line = NULL;
    size_t length;
    printf("purvis: ");
    getline(&line, &length, stdin);
    return line;
}

char ** parse(char *string, const char *delimiter) {
    char *currentToken;
    char **ptrList = NULL;
    size_t numTokens = 0;
    size_t maxTokens;

    if (string == NULL || strcmp(string, "") == 0) {
        ptrList = (char **)malloc(1 * sizeof(char*));
        ptrList[0] = NULL;
        return ptrList;
    }

    maxTokens = (strlen(string) / 2);  // estimating the amount of tokens
    ptrList = (char **)malloc(maxTokens * sizeof(char*));  

    currentToken = strtok(string, delimiter);
    while (currentToken != NULL) {
        ptrList[numTokens] = malloc(strlen(currentToken) + 1);  // +1 for null terminator
        strcpy(ptrList[numTokens], currentToken);               // copy token to allocated memory
        numTokens++;
        currentToken = strtok(NULL, delimiter);  
    }
    ptrList[numTokens] = NULL;
    return ptrList;
}

int isExitCommand(char **tokenList) {
    return (strcmp(tokenList[0], "exit") == 0 || strcmp(tokenList[0], "quit") == 0) && tokenList[1] == NULL;
}

int isCDCommand(char **tokenList) {
    return (strcmp(tokenList[0], "cd") == 0 || strcmp(tokenList[0], "chdir") == 0);
}

void executeCommands(char ***commandList) {
    pid_t pid;
    int status;

    for (int i = 0; commandList[i] != NULL; i++) {
        pid = fork();

        if (pid == 0) { // Child process
            // Execute the command
            execvp(commandList[i][0], commandList[i]);
            // If execvp fails
            fprintf(stderr, "purvis: %s: command not found\n", commandList[i][0]);
            exit(1);
        }
        // Parent process: do not wait immediately for child
    }
    waitpid(pid, &status, 0);
}

void freeListOfLists(char *** tokenList) {
    if (tokenList != NULL) {
        for (size_t i = 0; tokenList[i] != NULL; i++) {
            freeList(tokenList[i]);
        }
    }
    free(tokenList);
}

void freeList(char ** tokenList) {
    if (tokenList != NULL) {
        for (size_t i = 0; tokenList[i] != NULL; i++) {
            free(tokenList[i]);
        }
        free(tokenList);
    }
}

void printListOfLists(char ***listOfLists) {
    if (listOfLists == NULL) {
        printf("Prepped command list is empty.\n");
        return;
    }

    for (int i = 0; listOfLists[i] != NULL; i++) {  // Loop through each command
        printf("Command %d:\n", i);
        for (int j = 0; listOfLists[i][j] != NULL; j++) {  // Loop through each argument
            printf("  Arg %d: %s\n", j, listOfLists[i][j]);
        }
    }
}

