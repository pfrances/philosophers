# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/17 13:52:28 by pfrances          #+#    #+#              #
#    Updated: 2022/12/13 18:33:44 by pfrances         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE = -pthread
SRCS_DIR = srcs
OBJS_DIR = objs
SRCS =	$(addprefix $(SRCS_DIR)/,	main.c						\
									init.c						\
									set_nbr.c					\
									death_monitoring.c			\
									meals.c						\
									timming.c					\
									writing_logs.c)
OBJS = $(subst $(SRCS_DIR), $(OBJS_DIR), $(SRCS:.c=.o))
RM = rm -rf

ifdef ZURUI
	CFLAGS += -D ZURUI=1
endif

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

zurui:
	make all ZURUI=TRUE

.PHONY: all clean fclean re zurui