# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/05 00:58:35 by rboutaik          #+#    #+#              #
#    Updated: 2024/06/07 00:22:35 by rboutaik         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

FLAGS = -Wall -Wextra -Werror  #-fsanitize=thread -g

NAME = philo

CFILES = philo.c philo_utils.c philo_routine.c philo_utils_2.c philo_utils_3.c

OFILES = $(CFILES:.c=.o)

all : $(NAME)

$(NAME) : $(OFILES) philo.h
	$(CC) $(FLAGS) -o $@ $(OFILES)

%.o : %.c philo.h
	$(CC) $(FLAGS) -c $< -o $@

clean :
	rm -f $(OFILES)

fclean : clean
	rm -f $(NAME)

re : fclean all

