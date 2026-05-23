*This project has been created as part of the 42 curriculum by jomeltah, tkhamis*

# Minishell

> A simplified Unix shell reproducing essential Bash behavior through custom parsing, execution pipelines, redirections, builtins, and signal management.

---

# Description

## Project Overview

Minishell is a Unix system programming project whose objective is to recreate a simplified interactive shell inspired by Bash.

The program reads user commands, analyzes their syntax, expands environment variables, prepares redirections, executes builtins or external binaries, manages pipelines, and reacts correctly to terminal signals.

This project introduces the internal workflow of command interpreters and requires the implementation of a complete execution chain from raw input to process synchronization.

---

## Project Goals

Through Minishell, the following low-level concepts are explored:

- lexical analysis
- command parsing
- environment expansion
- process creation with `fork`
- binary execution with `execve`
- inter-process communication with `pipe`
- file descriptor manipulation with `dup2`
- asynchronous signal handling
- shell state preservation

The main objective is not only to execute commands, but to understand how a Unix shell internally interprets and orchestrates them.

---

# Work Distribution

This project was developed collaboratively with a clear subsystem separation.

## tkhamis (Tolay Khamis)— Parser Architecture

tkhamis was responsible for the full parsing subsystem, including:

- lexical token generation
- quote state management
- syntax validation
- operator recognition
- command structure creation
- redirection parsing
- pipeline command chaining
- environment expansion integration within parsing flow

This part established the entire structural foundation that transforms raw user input into executable command nodes.

---

## jomeltah (Joud Meltaha)— Execution, Builtins, and Signals

jomeltah was responsible for the runtime behavior of the shell, including:

- execution engine implementation
- external command launching
- PATH resolution
- pipe process orchestration
- file descriptor duplication and restoration
- builtin command implementation
- environment modifications in parent context
- heredoc execution behavior
- signal handling (`Ctrl-C`, `Ctrl-D`, `Ctrl-\`)
- child process synchronization and exit status management

This subsystem ensured that parsed commands behave consistently with expected Bash-like runtime behavior.

---

# Implemented Features

## Interactive Prompt
- command reading with `readline`
- history support

## Parsing System
- custom tokenizer
- syntax checker
- quote protection
- command linked-list generation

## Variable Expansion
- environment variables (`$VAR`)
- last exit status (`$?`)

## Redirections
- `<`
- `>`
- `>>`
- `<<`

## Pipes
- support for chained commands with `|`

## Builtins
- `echo`
- `cd`
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

## Signal Handling
- prompt interruption with `Ctrl-C`
- shell exit with `Ctrl-D`
- `Ctrl-\` behavior replication

## External Command Execution
- binary search inside `PATH`
- execution through `execve`

---

# Instructions

## Compilation

```bash
git clone https://github.com/tolaykhamis/minishell.git minishell
cd minishell
make
./minishell
```

# Resources

- What's Tokenization? https://www.geeksforgeeks.org/nlp/what-is-tokenization/
- Command Line Processing https://www.linux.org/threads/bash-03-%E2%80%93-command-line-processing.38676/
- Bash Parser https://mywiki.wooledge.org/BashParser
- Bash Explained https://www.digitalocean.com/community/tutorials/what-is-bash?fbclid=PARlRTSAR8V3dleHRuA2FlbQIxMABzcnRjBmFwcF9pZA8xMjQwMjQ1NzQyODc0MTQAAafEDd57O0LeLhoVLY77B5FztY-Q1xfbq7pNxN0zjroFUrzrc-1rxeQHpOy_ew_aem_s9CYc7qie4YsEJdpxogJ8g
- The Architecture of Open Source Application https://aosabook.org/en/v1/bash.html?fbclid=PARlRTSAR8V5NleHRuA2FlbQIxMABzcnRjBmFwcF9pZA8xMjQwMjQ1NzQyODc0MTQAAaczei39WLKytftikUjeQuVuAgeBR2bHS8hT1SnerNvgFh8lcSXVVQ1JFT5JCA_aem_yc5qKhwF_95KlJB9nIpMvw#:~:text=Bash%20reads%20input%20from%20the,Unix%20emacs%20and%20vi%20editors
- Bash Cookbook https://www.oreilly.com/library/view/bash-cookbook/0596526784/ch01s09.html?fbclid=PARlRTSAR8V2FleHRuA2FlbQIxMABzcnRjBmFwcF9pZA8xMjQwMjQ1NzQyODc0MTQAAafedtjdpdgxPd-7BBlJWszdYS2txyAqt_pQgnyJ5JAVmFcR10YMD9lavu2gtw_aem_mfzGXqhAONvpKEMb7TONMw

## AI Usage Disclosure

AI tools were used only for conceptual clarification, documentation support, and README improvement.

All implementation, debugging, testing, and final validation were completed manually by the project authors.






