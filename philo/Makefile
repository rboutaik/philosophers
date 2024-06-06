# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/05 00:58:35 by rboutaik          #+#    #+#              #
#    Updated: 2024/06/06 22:36:19 by rboutaik         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

FLAGS = -Wall -Wextra -Werror  #-fsanitize=thread -g

NAME = philo

CFILES = philo.c philo_utils.c

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

