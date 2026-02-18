NAME = libft.a

MY_SOURCES = ./ft_atoi.c ./ft_memchr.c ./ft_split.c ./ft_strncmp.c \
			./ft_bzero.c ./ft_memcmp.c ./ft_strchr.c ./ft_strnstr.c \
			./ft_calloc.c ./ft_memcpy.c ./ft_strdup.c ./ft_strrchr.c \
			./ft_isalnum.c ./ft_memmove.c ./ft_striteri.c ./ft_strtrim.c \
			./ft_isalpha.c ./ft_memset.c ./ft_strjoin.c ./ft_substr.c \
			./ft_isascii.c ./ft_putchar_fd.c ./ft_strlcat.c ./ft_tolower.c \
			./ft_isdigit.c ./ft_putendl_fd.c ./ft_strlcpy.c ./ft_toupper.c \
			./ft_isprint.c ./ft_putnbr_fd.c ./ft_strlen.c ./ft_itoa.c \
			./ft_putstr_fd.c ./ft_strmapi.c ./ft_itoa_unsigned.c ./ft_printf.c \
			./ft_printf_utils1.c ./ft_printf_utils2.c ./ft_putnbr_base.c \
			./get_next_line.c

MY_OBJECTS = $(MY_SOURCES:.c=.o)

BONUS_SOURCES = ./ft_lstnew_bonus.c ./ft_lstadd_front_bonus.c ./ft_lstsize_bonus.c \
			./ft_lstlast_bonus.c ./ft_lstadd_back_bonus.c ./ft_lstdelone_bonus.c \
			./ft_lstclear_bonus.c ./ft_lstiter_bonus.c ./ft_lstmap_bonus.c

BONUS_OBJECTS = $(BONUS_SOURCES:.c=.o)

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(MY_OBJECTS)
	ar rcs $(NAME) $(MY_OBJECTS)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

bonus: $(MY_OBJECTS) $(BONUS_OBJECTS)
	ar rcs $(NAME) $(MY_OBJECTS) $(BONUS_OBJECTS)

clean: 
	rm -f $(MY_OBJECTS) $(BONUS_OBJECTS)

fclean: clean
	rm -f $(NAME) $(EXEC)

re: fclean all
