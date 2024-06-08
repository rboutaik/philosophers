/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 00:58:42 by rboutaik          #+#    #+#             */
/*   Updated: 2024/06/08 08:38:57 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>
# include <semaphore.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>

typedef struct s_data
{
	int				philos_nbr;
	int				t_to_eat;
	int				t_to_sleep;
	int				t_to_die;
	int				eat_nbr;
	size_t			start_time;
	sem_t			*print;
	sem_t			*start_eating_lock;
	sem_t			*meals_lock;
	sem_t			*forks;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals;
	size_t			start_eating;
	pid_t			philo_pid;
	t_data			*data;
}	t_philo;

int		ft_atoi(const char *str);
int		args_failure(int flag);
int		args_checker(int ac, char **av, t_data *data);
int		ft_free(t_philo *philos);
void	custom_print(t_philo *philo, char *str);
size_t	get_current_time(void);
int		custom_usleep(size_t milliseconds, t_philo *philo);
void	*routine(t_philo *philo);
size_t	get_current_time(void);
int		custom_usleep(size_t milliseconds, t_philo *philo);
void	*philos_killer(t_philo *philos, int i);
int		create_philos(t_philo *philos, int i, t_data *data);
int		ft_destroy(int mut, t_data *data);
void	*ft_monitor(t_philo *philos);

#endif