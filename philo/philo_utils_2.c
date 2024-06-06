/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 00:09:52 by rboutaik          #+#    #+#             */
/*   Updated: 2024/06/07 00:35:07 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday fialed\n", 21);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	_died(t_philo *philo)
{
	int	bol;
	int	meals;

	pthread_mutex_lock(&philo->data->meals_lock);
	meals = philo->meals;
	pthread_mutex_unlock(&philo->data->meals_lock);
	if (philo->data->eat_nbr != -1 && meals >= philo->data->eat_nbr)
	{
		pthread_mutex_lock(&philo->data->total_meals_lock);
		philo->data->total_meals++;
		pthread_mutex_unlock(&philo->data->total_meals_lock);
		return (1);
	}
	pthread_mutex_lock(&philo->data->dead_lock);
	bol = philo->data->is_dead;
	pthread_mutex_unlock(&philo->data->dead_lock);
	return (bol);
}

int	custom_usleep(size_t milliseconds, t_philo *philo)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds && !_died(philo))
		usleep (100);
	return (0);
}

int	check_death(t_philo *philos, int i, int nb_philos)
{
	pthread_mutex_lock(&philos[i].data->total_meals_lock);
	if (philos[i].data->total_meals >= nb_philos)
	{
		pthread_mutex_unlock(&philos[i].data->total_meals_lock);
		return (1);
	}
	pthread_mutex_unlock(&philos[i].data->total_meals_lock);
	return (0);
}

void	*philos_killer(t_philo *philos, int i, int nb_philos)
{
	pthread_mutex_unlock(&philos[i].data->start_eating_lock);
	pthread_mutex_lock(&philos[i].data->dead_lock);
	philos[i].data->is_dead = 1;
	pthread_mutex_unlock(&philos[i].data->dead_lock);
	pthread_mutex_lock(&philos[i].data->print);
	if (check_death(philos, i, nb_philos))
		return (pthread_mutex_unlock(&philos[i].data->print), NULL);
	printf("%lu %d %s", get_current_time()
		- philos[i].data->start_time, i + 1, "died\n");
	pthread_mutex_unlock(&philos[i].data->print);
	return (NULL);
}
