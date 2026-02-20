NAME = minishell

SOURCES = $(addprefix $(SRC_DIR)/, \
		main.c lexing_1.c lexing_2.c lexing_del.c utils.c dquote.c)

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
