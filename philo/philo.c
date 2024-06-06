/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 00:58:39 by rboutaik          #+#    #+#             */
/*   Updated: 2024/06/07 00:29:17 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_monitor(t_philo *philos)
{
	int	i;
	int	nb_philos;

	nb_philos = philos[0].data->philos_nbr;
	while (1)
	{
		i = -1;
		while (++i < nb_philos)
		{
			if (check_death(philos, i, nb_philos))
				return (NULL);
			pthread_mutex_lock(&philos[i].data->start_eating_lock);
			if (get_current_time()
				- philos[i].start_eating >= (size_t)philos[i].data->t_to_die)
			{
				return (philos_killer(philos, i, nb_philos));
			}
			pthread_mutex_unlock(&philos[i].data->start_eating_lock);
			if (check_death(philos, i, nb_philos))
				return (NULL);
		}
	}
	return (NULL);
}

int	ft_destroy(pthread_mutex_t *forks, int n, int mut, t_data *data)
{
	int	i;

	i = -1;
	while (++i < n)
		pthread_mutex_destroy(&forks[i]);
	if (i == 0)
		free(forks);
	if (mut >= 1)
		pthread_mutex_destroy(&data->start_eating_lock);
	if (mut >= 2)
		pthread_mutex_destroy(&data->print);
	if (mut >= 3)
		pthread_mutex_destroy(&data->dead_lock);
	if (mut >= 4)
		pthread_mutex_destroy(&data->meals_lock);
	if (mut >= 5)
		pthread_mutex_destroy(&data->total_meals_lock);
	return (1);
}

int	mutex_init(t_data *data, pthread_mutex_t *forks, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < data->philos_nbr)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (ft_destroy(forks, i, 0, data), ft_free(philos, forks));
	}
	if (pthread_mutex_init(&data->start_eating_lock, NULL) != 0)
		return (ft_destroy(forks, i, 0, data), ft_free(philos, forks));
	if (pthread_mutex_init(&data->print, NULL) != 0)
		return (ft_destroy(forks, i, 1, data), ft_free(philos, forks));
	if (pthread_mutex_init(&data->dead_lock, NULL) != 0)
		return (ft_destroy(forks, i, 2, data), ft_free(philos, forks));
	if (pthread_mutex_init(&data->meals_lock, NULL) != 0)
		return (ft_destroy(forks, i, 3, data), ft_free(philos, forks));
	if (pthread_mutex_init(&data->total_meals_lock, NULL) != 0)
		return (ft_destroy(forks, i, 4, data), ft_free(philos, forks));
	data->forks = forks;
	data->total_meals = 0;
	i = -1;
	data->start_time = get_current_time();
	return (0);
}

int	philo_init(t_data *data)
{
	t_philo			*philos;
	pthread_mutex_t	*forks;
	int				i;

	philos = malloc(sizeof(t_philo) * (data->philos_nbr));
	if (philos == NULL)
		return (1);
	forks = malloc(sizeof(pthread_mutex_t) * (data->philos_nbr));
	if (forks == NULL)
		return (ft_free(philos, forks));
	if (mutex_init(data, forks, philos))
		return (1);
	i = -1;
	while (++i < data->philos_nbr)
	{
		if (create_philos(philos, i, forks, data))
			return (1);
	}
	(ft_monitor(philos), i = -1);
	while (++i < data->philos_nbr)
		if (pthread_join((philos + i)->philo, NULL) != 0)
			return (ft_free(philos, forks));
	return (ft_destroy(forks, i, 5, data), ft_free(philos, forks), 0);
}

int	main(int ac, char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (args_checker(ac, av, data))
		return (free(data), 1);
	if (philo_init(data))
		return (free(data), 1);
	free(data);
	return (0);
}
