# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/21 12:18:20 by mochegri          #+#    #+#              #
#    Updated: 2026/10/21 12:18:20 by mochegri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ft_ssl

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -Iinclude -Ilibft

LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

SRCS	= src/main.c \
		  src/commands.c \
		  src/digest/digest_cmd.c \
		  src/digest/digest_input.c \
		  src/digest/digest_output.c \
		  src/hash/hash_message.c \
		  src/hash/md5.c \
		  src/hash/sha256_core.c \
		  src/hash/sha256.c \
		  src/hash/sha512_core.c \
		  src/hash/sha512.c \
		  src/hash/whirlpool.c

OBJ_DIR	= obj
OBJS	= $(SRCS:%.c=$(OBJ_DIR)/%.o)
HEADERS	= include/ft_ssl.h include/digest.h include/hash.h $(LIBFT_DIR)/libft.h

LDLIBS	= -L$(LIBFT_DIR) -lft

# `make test` runs every suite, `make test md5 sha256` only those.
ALGOS		= md5 sha224 sha256 sha384 sha512 whirlpool
TEST_ALGOS	= $(filter $(ALGOS),$(MAKECMDGOALS))

all: libft $(NAME)

libft:
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: all
	@sh tests/run_tests.sh ./$(NAME) $(TEST_ALGOS)

# make compare S="string" [A="md5 sha256"]   or   make compare F=file
compare: all
	@sh tests/compare.sh $(A) $(if $(F),-f '$(F)',$(if $(S),-s '$(S)',--help))

norm:
	@norminette src include libft | grep -E "Error" \
		&& exit 1 || echo "norminette: all files OK"

ifneq ($(filter test,$(MAKECMDGOALS)),)
$(ALGOS):
	@:
endif

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all libft test compare norm clean fclean re $(ALGOS)
