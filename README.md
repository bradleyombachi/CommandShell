# Custom Command Shell

A custom command shell that supports both interactive and batch modes. It includes built-in commands such as `exit`, `cd`, and `alias`, as well as support for input/output redirection.

## Features

- **Interactive Mode:** Run the shell interactively, executing commands one at a time.
- **Batch Mode:** Read and execute commands from a script file in batch mode.
- **Built-in Commands:**
  - `exit`: Terminate the shell.
  - `cd`: Change the current working directory.
  - `alias`: Create and manage command aliases.
- **I/O Redirection:** Redirect input and output for commands.

## Getting Started

### Prerequisites

- Your system should have a C compiler installed (e.g., GCC).

### Build

```bash
make
```

## Run Interactive Mode

```bash
./newshell

```
## Run Batch Mode

```bash
./newshell batchFile.txt
```

## Usage
### Interactive Mode
1. Launch the shell.
2. Enter commands interactively.
3. Use built-in commands (exit, cd, alias) and I/O redirection.
 ```bash
$ ./newshell
Shell> ls -l
... (output)
Shell> cd /path/to/directory
Shell> exit
```
### Batch Mode
1. Prepare a script file with commands.
2. Run the shell with the script file.
```bash
./newshell batchFile.txt
```

## Built-in Commands
1. `exit`
Terminate the shell. If the exit command is on the same line with other commands, the other commands will execute (and finish) before shell is exited.
```bash
Shell> exit
```
2. `cd`
Change the current working directory.
```bash
Shell> cd /path/to/directory
```
3. `alias`
Define shortcuts for commands using `alias alias_name='command'`. Use alias with no arguments to display all existing aliases. Remove a single alias with `alias -r alias_name` or all defined aliases with `alias -c`. This custom alias command substitutes a given string for a command, resembling the bash alias but implemented as a built-in command for proper shell execution control. 
```bash
Shell> alias 5='ls -al'
Shell> 5
... (output of 'ls -al' command)
```
## I/O Redirection
Redirect input and output for commands.
```bash
Shell> ls > output.txt
Shell> cat < input.txt
```

   

