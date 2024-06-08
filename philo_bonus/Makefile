# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/05 00:58:35 by rboutaik          #+#    #+#              #
#    Updated: 2024/06/08 08:28:57 by rboutaik         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

FLAGS = -Wall -Wextra -Werror

NAME = philo_bonus

CFILES = philo_bonus.c philo_utils_bonus.c philo_routine_bonus.c philo_utils_2_bonus.c philo_utils_3_bonus.c

OFILES = $(CFILES:.c=.o)

all : $(NAME)

$(NAME) : $(OFILES) philo_bonus.h
	$(CC) $(FLAGS) -o $@ $(OFILES)

%.o : %.c philo_bonus.h
	$(CC) $(FLAGS) -c $< -o $@

clean :
	rm -f $(OFILES)

fclean : clean
	rm -f $(NAME)

re : fclean all

