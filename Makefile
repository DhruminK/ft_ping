# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/09 16:19:24 by dkhatri           #+#    #+#              #
#    Updated: 2023/06/26 19:17:42 by dkhatri          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror
NAME = ft_ping

PARSER = ft_socket.c
SOLVER = ft_ping_icmp_echo_req.c ft_ping_icmp_echo_reply.c ft_ping_once.c
HELPER_FUNC = helper_func.c icmp_helper.c ft_newton.c
PRINT = ft_print.c ft_print_helper.c
MAIN = main.c

SRC = $(PARSER) $(SOLVER) $(HELPER_FUNC) $(PRINT) $(MAIN)
OBJ = $(SRC:.c=.o)

INC = ping.h icmp.h
INCLUDES = -I include/

SRC_PATH = src/
INC_PATH = include/
OBJ_PATH = obj/

OBJS = $(addprefix $(OBJ_PATH), $(OBJ))
SRCS = $(addprefix $(SRC_PATH), $(SRC))
INCS = $(addprefix $(INC_PATH), $(INC))

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INCS)
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	@$(CC) $(INCLUDES) $(CFLAGS) -o $@ -c $<
	@echo "\033[1m$(NAME)\033[0m \
		: Compiling $< to $@ \033[1;32m[OK]\033[0m"

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(INCLUDES) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[1m$(NAME)\033[0m \
		: Compiling $@ \033[1;32m[OK]\033[0m"

clean:
	@/bin/rm -f $(OBJS)
	@echo "\033[1m$(NAME)\033[0m \
		: Cleaned $(OBJ_PATH) \033[1;32m[OK]\033[0m"

fclean: clean
	@/bin/rm -f $(NAME)
	@/bin/rm -rf $(OBJ_PATH)
	@echo "\033[1m$(NAME)\033[0m \
		: binary deleted \033[1;32m[OK]\033[0m"

re: fclean all

.PHONY: all clean fclean re
