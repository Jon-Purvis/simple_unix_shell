# Assignment 2 - My Simple Shell

## Completed Features

1. **Display a prompt**  
   - **Working:** Yes, the prompt displays as `purvis:`.

2. **Read a command line from standard input**  
   - **Working:** Yes, commands are read from the keyboard.

3. **Start a process to execute the command and wait until the process completes**  
   - **Working:** Yes, the shell forks and waits for the child process to complete.

4. **Display another prompt and go back to step 2**  
   - **Working:** Yes, the prompt is redisplayed after each command completes.

## Advanced Features

1. **Run a Program in Background**  
   - **Working:** Yes, the shell executes commands followed by `#` in the background, allowing continued user input while the background command runs.

2. **Parallel Commands**  
   - **Working:** Yes, multiple commands separated by `#` are executed in parallel, with the shell waiting only for the last program to complete.

## Incomplete or Non-Working Features

1. **CD ""/ CD ~**  
   - **Issue:** cd/chdir alone or with the tilde does not bring you to the home directory. I am unsure how to fix.


## C Function Descriptions

### `getCommands()`
- **Purpose:** Retrieves and parses user input into a list of commands split by `#`.
- **Input:** None.
- **Expected Output:** List of command lists (each with command and arguments).

### `readInput()`
- **Purpose:** Prompts the user for input and returns it.
- **Input:** None.
- **Expected Output:** A dynamically allocated string containing the input.

### `parse(char *string, const char *delimiter)`
- **Purpose:** Splits the input string into tokens based on the delimiter.
- **Input:** A string and a delimiter.
- **Expected Output:** List of tokens, ending with `NULL`.

### `isExitCommand(char **tokenList)`
- **Purpose:** Checks if the command is "exit" or "quit" with no arguments.
- **Input:** A list of tokens.
- **Expected Output:** Returns 1 if it's an exit command, otherwise 0.

### `isCDCommand(char **tokenList)`
- **Purpose:** Checks if the command is `cd` and changes the directory.
- **Input:** A list of tokens.
- **Expected Output:** Returns 1 if it's a `cd` command, otherwise 0.

### `executeCommands(char ***commandList)`
- **Purpose:** Forks a child process and executes the command using `execvp()`.
- **Input:** List of command lists.
- **Expected Output:** Executes the command and waits for completion.

### `freeListOfLists(char ***tokenList)`
- **Purpose:** Frees memory used by a list of command lists.
- **Input:** A list of command lists.
- **Expected Output:** Frees allocated memory.

### `freeList(char **tokenList)`
- **Purpose:** Frees memory used by a list of tokens.
- **Input:** A list of tokens.
- **Expected Output:** Frees allocated memory.
