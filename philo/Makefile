# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dberreby <dberreby@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/14 11:38:53 by dberreby          #+#    #+#              #
#    Updated: 2023/07/03 18:03:54 by dberreby         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC				=	philo_main.c init.c utils.c actions.c check_death_eat.c

OBJ 			=	$(SRC:.c=.o)

CC				= cc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror
NAME			= philo

all:			$(NAME)

%.o: %.c
				$(CC) -o $@ -c $< $(CFLAGS)

$(NAME):		$(OBJ)
					$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
				$(RM) $(OBJ)

fclean:			clean
					$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re

