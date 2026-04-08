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
Lexing is the first step of the program. It takes the raw input string and breaks it into a linked list of typed tokens. Spaces are skipped, operator characters (|, <, >, <<, >>) become their corresponding token type, and everything else is read as a WORD token.
A key feature of our lexer is that it tracks the quote context of every character. Each word token carries a quoting array of the same length as its text, where each character is tagged as NONE, SINGLE, DOUBLE, or SPECIAL. This allows the expansion stage to know exactly which characters were inside quotes and should not be expanded. Unclosed quotes are detected and reported as a syntax error at this stage.

# Parsing
Parsing takes the token list produced by the lexer and builds a structured list of commands. Each command node contains its arguments and its redirections.

The parser walks through the token list and uses | tokens as separators — each segment between two pipes becomes one command node. For each segment, get_args collects all WORD tokens that are not redirection filenames, and get_redirs collects all redirection operators together with the filename that follows them. Both results are stored in a t_cmd_list node which is then added to the command list.

At the end of parsing, data->cmd_list contains one node per command, ready to be passed to the expansion stage.

# Expanding
Expansion is the step that runs after parsing and transforms the abstract command structure into concrete, executable arguments. It takes the t_cmd_list (which contains t_word objects with both text and quoting metadata) and produces a t_expanded_list (plain char ** arrays ready for execve).

Argument expansion : reads each word character by character. When it finds a $, it looks up the variable and replaces it with its value. If the value contains spaces, it gets split into multiple arguments.

Redirection expansion works the same way but never splits on spaces — a filename always stays one single word.

# Instructions

Clone the repository, then run :
make
./minishell

To clean compiled files :
bash

make clean   # removes object files
make fclean  # removes object files and the binary
make re      # full recompile from scratch

# Limitations
We did not implement the Bonus part. 
- No support for &&, ||
- No support for positional parameters ($1, $2...)
- Must be run in an interactive terminal (isatty check on stdin/stdout)

# Resources
In order to hep us understand how the buit in functions work exactly, we used the documentation available online, especially the Linux Manua pages website.

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

# AI Usage :
We used AI as a learning and debugging aid throughout the project, specifically for :

- Understanding allowed functions — many of the authorized functions (readline, rl_clear_history, rl_on_new_line,
rl_replace_line, rl_redisplay, sigaction, etc.) were unfamiliar to us, and we used AI to understand what they do and how to use them correctly.
- Understanding concepts — AI helped us grasp the overall architecture of the shell, particularly the lexing, parsing, and expansion stages.
- Debugging — when we encountered errors, we used AI to help us understand what was going wrong and get hints on how to fix it.
- Edge cases — AI helped us identify and understand edge cases we might have missed.

At no point did AI write code for us directly.

