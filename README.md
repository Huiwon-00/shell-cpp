# cpp-shell

A small Unix shell written from scratch in C++23. It reads a command, splits it
into tokens, parses it into commands, and runs them, using the same system calls
real shells use: `fork`, `execv`, `pipe`, `dup2` and `waitpid`.

```
$ echo hello | tr a-z A-Z
HELLO
$ ls /dev | head -3 | wc -l
3
$ type ls > out.txt
$ cat out.txt
ls is /usr/bin/ls
```

## Features

- **Builtins:** `echo`, `pwd`, `cd` (including `cd` and `cd ~` for home), `type`, `exit`
- **External programs:** finds them by searching each folder in `PATH`
- **Pipelines:** any number of commands, e.g. `cmd1 | cmd2 | cmd3`
- **Redirection:**
  - `>` / `1>` overwrite stdout, `>>` / `1>>` append stdout
  - `2>` overwrite stderr, `2>>` append stderr
  - works on builtins, external programs and commands inside a pipeline
- **Quoting:** single quotes, double quotes and backslash escapes
- **Error handling:** syntax errors (e.g. `| ls`), "command not found",
  and failed redirects (`echo hi > /missing/file` reports the error and
  does not run the command)

## Build and run

Requirements: Linux (or WSL), CMake 3.13+, a C++23 compiler (e.g. g++ 13+), and
the readline library (`sudo apt install libreadline-dev`).

```bash
cmake -S . -B build      # configure (first time, or after adding a .cpp file)
cmake --build build      # compile
./build/shell            # run; type `exit` or press Ctrl+D to quit
```

## How it works

Each line goes through three stages:

1. **Tokenizer** (`tokenizer.cpp`): splits the line into tokens and handles
   quotes and escapes. Operators become typed tokens (`Pipe`, `RedirectOut`,
   `AppendErr`, ...) so later stages never have to re-check strings.
2. **Parser** (`parser.cpp`): turns the tokens into a `Pipeline`, a list of
   `Command`s. Each `Command` holds its arguments and its redirect files.
   Syntax errors are caught here.
3. **Executor** (`main.cpp`, `builtin.cpp`, `pipeline.cpp`, `redirect.cpp`):
   - **One builtin** runs inside the shell process itself, so `cd` and `exit`
     can change the shell. Redirects are applied, then stdout/stderr are
     restored afterwards.
   - **One external program** is run in a child process (`fork` + `execv`),
     and the redirects are applied in the child.
   - **A pipeline** forks one child per command, connects them with pipes,
     starts all of them, and then waits for all of them.

## Project structure

```
src/
├── main.cpp        read-eval loop; decides builtin / external / pipeline
├── tokenizer.*     line -> tokens
├── parser.*        tokens -> Pipeline of Commands
├── command.h       the Command struct and Pipeline type
├── builtin.*       echo, pwd, cd, type, and running external programs
├── pipeline.*      running commands connected by pipes
└── redirect.*      opening files and pointing stdout/stderr at them
```

## Possible next steps

- Tab completion for commands (readline is already linked)
- Command history
- Exit status and `$?`
