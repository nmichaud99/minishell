NAME = minishell

SOURCES = $(addprefix $(SRC_DIR)/, \
		utils.c exit.c \
		lexing_1.c lexing_2.c parsing.c syntax_check.c expansion.c \
		built_ins_export.c built_ins_unset.c built_ins_echo.c built_ins_env.c built_ins_cd.c \
		main.c perror_handling.c built_ins_pwd.c)

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
