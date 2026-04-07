A “Description” section that clearly presents the project, including its goal and a
brief overview.
• An “Instructions” section containing any relevant information about compilation,
installation, and/or execution.
• A “Resources” section listing classic references related to the topic (documen-
tation, articles, tutorials, etc.), as well as a description of how AI was used —
specifying for which tasks and which parts of the project.
➠ Additional sections may be required depending on the project (e.g., usage
examples, feature list, technical choices, etc.).


This project has been created as part of the 42 curriculum by nmichaud, fjerrige

# Description
The goal of the Minishell project was to recreate our own version of shell.

# Features
- Command execution with absolute and relative paths
- Pipes (|) chaining multiple commands
- Redirections : <, >, >>, << (heredoc)
- Environment variable expansion ($VAR, $?)
- Quote handling : single quotes (no expansion) and double quotes (expansion)
- Built-in commands : echo, cd, pwd, export, unset, env, exit
- Signal handling : Ctrl+C, Ctrl+D, Ctrl+\
- Command history

### How does it work ?
In order to work correctly, the code execution is the divided into the following steps :

# Lexing
This is the first step of the program. The objective is to take the raw input string typed by the user and break it into a list of tokens — the smallest meaningful units. Each token has a type and possibly a value.

# Parsing
Once the lexing, is done, we proceed with the parsing step. Here, we take the token list and we build a linked list.

# Expanding
Expansion is the step that runs after parsing and transforms the abstract command structure into concrete, executable arguments. It takes the t_cmd_list (which contains t_word objects with both text and quoting metadata) and produces a t_expanded_list (plain char ** arrays ready for execve).

Argument expansion : reads each word character by character. When it finds a $, it looks up the variable and replaces it with its value. If the value contains spaces, it gets split into multiple arguments.

Redirection expansion works the same way but never splits on spaces — a filename always stays one single word.

# Execution


# Instructions

Usage : ./minishell

minishell$ echo "Hello World !"

# Limitations
We did not implement the Bonus part. 
- No support for &&, ||
- No support for positional parameters ($1, $2...)
- Must be run in an interactive terminal (isatty check on stdin/stdout)

# Resources
Linux manual pages :
- 'export' man page : https://www.man7.org/linux/man-pages/man1/export.1p.html
- 'cd' man page : https://man7.org/linux/man-pages/man1/cd.1p.html
- 'unset' man page : https://man7.org/linux/man-pages/man1/unset.1p.html
- 'env' man page : https://man7.org/linux/man-pages/man1/env.1.html
- 'echo' man page : https://man7.org/linux/man-pages/man1/echo.1.html
- 'exit' man page : https://man7.org/linux/man-pages/man3/exit.3.html
- 'pwd' man page : https://man7.org/linux/man-pages/man1/pwd.1.html

Articles (Medium, etc...) :
- https://medium.com/@mostafa.omrane/mes-conseils-si-je-devais-recommencer-minishell-a9783c51ba1b