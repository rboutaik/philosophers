/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 00:08:59 by rboutaik          #+#    #+#             */
/*   Updated: 2024/06/07 00:33:47 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	think(t_philo *philo)
{
	custom_print(philo, "is thinking\n");
}

int	eat_2(t_philo *philo)
{
	custom_print(philo, "has taken a fork\n");
	if (_died(philo))
	{
		pthread_mutex_unlock(&philo->data->forks[(philo->id + 1)
			% philo->data->philos_nbr]);
		return (pthread_mutex_unlock(&philo->data->forks[philo->id]), 1);
	}
	custom_print(philo, "is eating\n");
	pthread_mutex_lock(&philo->data->start_eating_lock);
	philo->start_eating = get_current_time();
	pthread_mutex_unlock(&philo->data->start_eating_lock);
	if (_died(philo))
	{
		pthread_mutex_unlock(&philo->data->forks[(philo->id + 1)
			% philo->data->philos_nbr]);
		return (pthread_mutex_unlock(&philo->data->forks[philo->id]), 1);
	}
	custom_usleep(philo->data->t_to_eat, philo);
	pthread_mutex_lock(&philo->data->meals_lock);
	philo->meals++;
	pthread_mutex_unlock(&philo->data->meals_lock);
	pthread_mutex_unlock(&philo->data->forks[philo->id]);
	pthread_mutex_unlock(&philo->data->forks[(philo->id + 1)
		% philo->data->philos_nbr]);
	return (0);
}

int	eat(t_philo *philo)
{
	if (_died(philo))
		return (1);
	pthread_mutex_lock(&philo->data->forks[philo->id]);
	if (_died(philo))
		return (pthread_mutex_unlock(&philo->data->forks[philo->id]));
	custom_print(philo, "has taken a fork\n");
	if (philo->data->philos_nbr == 1)
		custom_usleep(philo->data->t_to_die, philo);
	if (_died(philo))
	{
		pthread_mutex_unlock(&philo->data->forks[philo->id]);
		return (1);
	}
	pthread_mutex_lock(&philo->data->forks[(philo->id + 1)
		% philo->data->philos_nbr]);
	if (_died(philo))
	{
		pthread_mutex_unlock(&philo->data->forks[philo->id]);
		pthread_mutex_unlock(&philo->data->forks[(philo->id + 1)
			% philo->data->philos_nbr]);
		return (1);
	}
	return (eat_2(philo));
}

void	_sleep(t_philo *philo)
{
	if (_died(philo))
		return ;
	custom_print(philo, "is sleeping\n");
	if (_died(philo))
		return ;
	custom_usleep(philo->data->t_to_sleep, philo);
}

void	*routine(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(2000);
	while (1)
	{
		if (_died(philo))
			return (NULL);
		think(philo);
		eat(philo);
		_sleep(philo);
	}
	return (NULL);
}
