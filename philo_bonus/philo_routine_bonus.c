/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 00:08:59 by rboutaik          #+#    #+#             */
/*   Updated: 2024/06/08 08:18:51 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	think(t_philo *philo)
{
	custom_print(philo, "is thinking\n");
}

int	eat_2(t_philo *philo)
{
	custom_print(philo, "has taken a fork\n");
	custom_print(philo, "is eating\n");
	sem_wait(philo->data->start_eating_lock);
	philo->start_eating = get_current_time();
	sem_post(philo->data->start_eating_lock);
	custom_usleep(philo->data->t_to_eat, philo);
	sem_wait(philo->data->meals_lock);
	philo->meals++;
	sem_post(philo->data->meals_lock);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	return (0);
}

int	eat(t_philo *philo)
{
	sem_wait(philo->data->forks);
	custom_print(philo, "has taken a fork\n");
	if (philo->data->philos_nbr == 1)
		custom_usleep(philo->data->t_to_die, philo);
	sem_wait(philo->data->forks);
	return (eat_2(philo));
}

void	_sleep(t_philo *philo)
{
	custom_print(philo, "is sleeping\n");
	custom_usleep(philo->data->t_to_sleep, philo);
}

void	*routine(t_philo *philo)
{
	pthread_t	monitor;
	int			meals;

	if (pthread_create(&monitor, NULL, (void *)ft_monitor, (void *)philo) != 0)
		exit(2);
	if (pthread_detach(monitor) != 0)
		exit(2);
	while (1)
	{
		think(philo);
		eat(philo);
		_sleep(philo);
		sem_wait(philo->data->meals_lock);
		meals = philo->meals;
		sem_post(philo->data->meals_lock);
		if (philo->data->eat_nbr != -1 && meals >= philo->data->eat_nbr)
		{
			exit(0);
		}
	}
	exit(0);
}
