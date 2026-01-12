# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/12 22:08:00 by asmaili           #+#    #+#              #
#    Updated: 2026/01/12 22:17:24 by asmaili          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= fdf

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g
INCLUDES= -Iincludes

SRCS	= main.c \
		  srcs/parse.c \
		  srcs/draw.c \
		  srcs/projection.c \
		  srcs/events.c \
		  srcs/utils.c

OBJS	= $(SRCS:.c=.o)

LIBS	= -Llib -lft -lmlx -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
