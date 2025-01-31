# CMocka Library
CMOCKA_DIR := cmocka
CMOCKA_BUILD := $(CMOCKA_DIR)/build
CMOCKA_INCLUDE := $(CMOCKA_DIR)/include
CMOCKA_LIB := $(CMOCKA_BUILD)/src/libcmocka.so

# Test Executable 
CC := cc
CFLAGS := -Wall -Wextra -Werror -I $(CMOCKA_INCLUDE)
ifeq ($(DEBUG), TRUE)
	CFLAGS += -g
endif
LDFLAGS := -lbsd -Wl,--wrap=malloc -Wl,--wrap=write -Wl,--wrap=free
NAME := test
SRCS := main.c
OBJS := $(SRCS:.c=.o)

### RULES ######################################################################

all: run_test

run_test: source $(NAME)
	@LD_LIBRARY_PATH=$(shell dirname $(CMOCKA_LIB)) ./$(NAME)

$(NAME): $(CMOCKA_LIB) $(OBJS) ../libft.a
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

../libft.a: source

source: 
	@cd ../ && $(MAKE) bonus -s

$(CMOCKA_LIB):
	@git submodule update --init --recursive
	@mkdir -p $(CMOCKA_BUILD)
	@cd $(CMOCKA_BUILD) && cmake -DCMAKE_BUILD_TYPE=Release ..
	@$(MAKE) -C $(CMOCKA_BUILD)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS) $(CMOCKA_BUILD)

fclean: clean
	@rm -f $(NAME) 

re: fclean all

.PHONY: all clean fclean re run_test source
