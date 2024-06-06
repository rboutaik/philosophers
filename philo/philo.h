/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 00:58:42 by rboutaik          #+#    #+#             */
/*   Updated: 2024/06/06 23:05:28 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

typedef struct s_data
{
	int 			philos_nbr;
	int 			t_to_eat;
	int 			t_to_sleep;
	int 			t_to_die;
	int 			eat_nbr;
	size_t			start_time;
	int				is_dead;
	int				total_meals;
	pthread_mutex_t	print;
	pthread_mutex_t	total_meals_lock;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	start_eating_lock;
	pthread_mutex_t	meals_lock;
	pthread_mutex_t	*forks;
	
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals;
	size_t			start_eating;
	pthread_t		philo;
	t_data			*data;
}	t_philo;

int		ft_atoi(const char *str);
int		args_failure(int flag);
int		args_checker(int ac, char **av, t_data *data);
int		malloc_failure(int n, t_philo *philos, pthread_mutex_t *forks);
void	custom_print(t_philo *philo, char *str);
size_t	get_current_time(void);
int		_died(t_philo *philo);
int		custom_usleep(size_t milliseconds, t_philo *philo);

#endif