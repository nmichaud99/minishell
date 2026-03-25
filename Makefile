NAME = minishell

SOURCES = $(addprefix $(SRC_DIR)/, \
		utils.c exit_1.c exit_2.c exit_3.c f_printf.c env_utils.c \
		lexing_handle_op.c lexing_handle_words.c lexing_utils.c lexing.c syntax_check.c \
		parsing_args.c parsing_utils.c parsing.c \
		expansion_args.c expansion_utils.c expansion.c \
		built_ins_cd.c built_ins_echo.c built_ins_env.c built_ins_export_1.c built_ins_export_2.c \
		built_ins_unset.c built_ins_exit.c built_ins_utils.c \
		redir_handler.c redir_heredoc_expansion.c \
		pipe.c exec_commands_1.c exec_commands_2.c get_full_path.c \
		main.c)

OBJECTS = $(SOURCES:src/%.c=obj/%.o)

SRC_DIR = src

CFLAGS = -Wall -Wextra -Werror -Iincludes

LIBS = -lreadline -lhistory

all : libft $(NAME)

$(NAME) : libft/libft.a $(OBJECTS)
	cc $(CFLAGS) $(OBJECTS) libft/libft.a $(LIBS) -o $(NAME)

obj/%.o: src/%.c
	mkdir -p obj
	cc $(CFLAGS) -c $< -o $@

libft:
	$(MAKE) -C libft

clean:
	rm -f $(OBJECTS)
	$(MAKE) -C libft clean

fclean: clean
	rm -rf $(NAME)
	$(MAKE) -C libft fclean

re: fclean all

.PHONY: all clean libft
