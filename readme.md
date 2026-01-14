# Ashell — Custom Unix Shell in C

Ashell is a Unix-like shell implemented in C.  
It supports built-in commands, external command execution, pipes, redirections, background processes, and command history.

The project is structured to reflect real shell architecture: input reading, tokenization, parsing, and execution are cleanly separated.

---

## Features

- Built-in commands:
  - `cd`
  - `echo`
  - `pwd`
  - `exit`
  - `env`
  - `export`
  - `unset`
  - `history`

- External command execution using `execvp`
- Pipes (`|`) with support for multiple stages
- Input and output redirection:
  - `<`, `>`, `>>`
- Background execution using `&`
- Command history
- Modular architecture:
  - Reader
  - Tokenizer
  - Parser
  - Executor
  - Builtins / Externals

---

## Project Structure

.
├── main.c

├── reader.c

├── tokenizer.c

├── parser.c

├── executor.c

├── built_ins.c

├── externals.c

├── utils.c

├── includes.h

└── build.sh


---

## Build

A build script is provided.

### Using the build script

```bash
chmod +x build.sh
./build.sh

This will compile all source files and produce the binary:

Ashell

Manual compilation

gcc -Wall -Wextra -Werror *.c -o Ashell

Run

./Ashell

Usage Examples

pwd
cd src
ls -la
echo hello world
ls | grep ".c" | wc -l
cat input.txt | grep error > output.txt
cat input.txt | grep error >> log.txt
sleep 5 &

Architecture Overview

Ashell follows a standard shell execution pipeline:

Input
  ↓
Reader        → reads user input
  ↓
Tokenizer     → converts input into tokens
  ↓
Parser        → builds t_cmd linked list (commands, pipes, redirections)
  ↓
Executor      → handles:
                  - single commands
                  - pipelines
                  - redirections
                  - background execution
                  - builtins vs externals
  ↓
Builtins / External commands

This design keeps responsibilities separated and simplifies maintenance and extension.
Built-in Commands

Builtins are executed internally without spawning a new process.
Dispatch is handled through:

int is_builtin(char *cmd);
void run_builtin(t_cmd *cmd, t_history **history);

External Commands

External commands are executed using:

execvp(argv[0], argv);

This provides standard PATH resolution and behavior consistent with typical Unix shells.
Notes

    All memory allocations are paired with proper cleanup.

    The project is written in standard C and uses POSIX system calls.

    The codebase is organized for clarity and extensibility.
