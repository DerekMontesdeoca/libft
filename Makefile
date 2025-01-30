CC := cc
CFLAGS := -Wall -Wextra -Werror
ifeq ($(DEBUG), TRUE)
	CFLAGS += -g
endif
NAME := libft.a

HEADERS := libft.h char_classes.h
SRCS := ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_strlen.c \
		ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
		ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c \
		ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c \
		ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c \
		ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_isprint.c \
		ft_putnbr_fd.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c \
		char_classes.c
OBJS := $(SRCS:.c=.o)

BONUS_SRCS := ft_lst_1_bonus.c ft_lst_2_bonus.c
BONUS_OBJS := $(BONUS_SRCS:.c=.o)

################################################################################
# Rules                                                                        #
################################################################################

all: $(NAME)

$(NAME): $(OBJS)
	ar rcsv $@ $?

bonus: $(NAME) $(BONUS_OBJS)
	ar rcsv $(NAME) $(BONUS_OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus

