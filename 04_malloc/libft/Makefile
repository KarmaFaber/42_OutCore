# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/13 12:45:25 by KarmaFaber        #+#    #+#              #
#    Updated: 2026/11/13 12:47:14 by KarmaFaber       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables
NAME	=	libft.a
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror
ARFLAGS	=	ar rcs
RM		=	rm -rf 

# Functions files
SRC		=	ft_bzero.c \
			ft_count_dig_unsigned.c \
			ft_memcpy.c \
			ft_memmove.c \
			ft_memset.c \
			ft_putchar_fd.c \
			ft_putendl_fd.c \
			ft_putstr_fd.c \
			ft_strcmp.c \
			ft_strlen.c \
			ft_strncmp.c \

OBJS		=	$(SRC:.c=.o)

# Rules
all: $(NAME)
	@echo "\033[0;34mStatic library '$(NAME)' created.\n\033[0m"

$(NAME): $(OBJS)
	$(ARFLAGS) $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	@$(RM) $(OBJS)
	@echo "\033[0;34mObject files '$(NAME)' removed.\n\033[0m"
	
fclean: clean
	@$(RM) $(NAME)
	@echo "\033[0;34mStatic library '$(NAME)' removed.\n\033[0m"

re: fclean all 

# Phony declarations
.PHONY: all clean fclaen re
