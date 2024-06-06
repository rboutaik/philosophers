/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 00:58:39 by rboutaik          #+#    #+#             */
/*   Updated: 2024/06/06 19:29:48 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	custom_usleep(size_t milliseconds, t_data *data)
{
	size_t	start;

	(void)data;
	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep (100);
	return (0);
}

void	think(t_philo *philo)
{
	custom_print(philo, "is thinking\n");
}
void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->id]);
	custom_print(philo, "has taken a fork\n");

	pthread_mutex_lock(&philo->data->forks[(philo->id + 1) % philo->data->philos_nbr]);
	custom_print(philo, "has taken a fork\n");
	


	pthread_mutex_lock(&philo->data->start_eating_lock);
	philo->start_eating = get_current_time();
	pthread_mutex_unlock(&philo->data->start_eating_lock);
	
	custom_print(philo, "is eating\n");
	custom_usleep(philo->data->t_to_eat, philo->data);
	pthread_mutex_unlock(&philo->data->forks[philo->id]);
	pthread_mutex_unlock(&philo->data->forks[(philo->id + 1) % philo->data->philos_nbr]);
	
	
}

void	_sleep(t_philo *philo)
{
	custom_print(philo, "is sleeping\n");
	custom_usleep(philo->data->t_to_sleep, philo->data);
}


void	*routine(t_philo *philo)
{
	int	bol;

	bol  = 0;
	while (!bol)
	{
		pthread_mutex_lock(&philo->data->start_routine_lock);
		bol = philo->data->start_routine;
		pthread_mutex_unlock(&philo->data->start_routine_lock);
	}

	pthread_mutex_lock(&philo->data->start_eating_lock);
	philo->start_eating = get_current_time();
	pthread_mutex_unlock(&philo->data->start_eating_lock);

	pthread_mutex_lock(&philo->data->start_monitor_lock);
	philo->data->start_monitor++;
	pthread_mutex_unlock(&philo->data->start_monitor_lock);
	while (1)
	{
		
		think(philo);
		eat(philo);
		_sleep(philo);

	}
	return (NULL);
}


void	*ft_monitor(t_philo *philos)
{
	int	i;
	int nb_philos;
	int	bol;

	i = 0;
	bol = 0;
	nb_philos = philos[i].data->philos_nbr;
	while (bol < nb_philos)
	{
		pthread_mutex_lock(&philos[i].data->start_monitor_lock);
		bol = philos[i].data->start_monitor;
		pthread_mutex_unlock(&philos[i].data->start_monitor_lock);
	}
	while (1)
	{
		i = 0;
		while (i < nb_philos)
		{
			pthread_mutex_lock(&philos[i].data->start_eating_lock);
			if (get_current_time() - philos[i].start_eating > (size_t)philos[i].data->t_to_die)
			{
				
				pthread_mutex_unlock(&philos[i].data->start_eating_lock);
				pthread_mutex_lock(&philos[i].data->print);
				printf("%lu %d %s", get_current_time() - philos[i].data->start_time, i + 1, "died\n");
				return (NULL);
			}
			pthread_mutex_unlock(&philos[i].data->start_eating_lock);
			
			i++;
		}
	}
	return (NULL);
}

int	philo_init(t_data *data)
{
	t_philo			*philos;
	pthread_mutex_t	*forks;
	int				i;
	pthread_t		monitor;

	i = -1;
	philos = malloc(sizeof(t_philo) * (data->philos_nbr));
	if (philos == NULL)
		return(1);
	forks = malloc(sizeof(pthread_mutex_t) * (data->philos_nbr));
	if (forks == NULL)
		return(malloc_failure(0, philos, forks));


	while (++i < data->philos_nbr)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (malloc_failure(i, philos, forks));
	}
	if (pthread_mutex_init(&data->start_eating_lock, NULL) != 0)
		return (malloc_failure(i, philos, forks));
	if (pthread_mutex_init(&data->print, NULL) != 0)
		return (malloc_failure(i, philos, forks));
	if (pthread_mutex_init(&data->start_routine_lock, NULL) != 0)
		return (malloc_failure(i, philos, forks));
	if (pthread_mutex_init(&data->start_monitor_lock, NULL) != 0)
		return (malloc_failure(i, philos, forks));
	data->forks = forks;
	i = -1;
	data->start_routine = 0;
	data->start_monitor = 0;
	while (++i < data->philos_nbr)
	{
		(philos + i)->id = i;
		(philos + i)->data = data;
		(philos + i)->meals = 0;
		if (pthread_create(&(philos + i)->philo, NULL, (void *)routine, (void *)(philos + i)) != 0)
			return (malloc_failure(data->philos_nbr, philos, forks));
	}
	
	

	if (pthread_create(&monitor, NULL, (void *)ft_monitor, (void *)philos) != 0)
			return (malloc_failure(data->philos_nbr, philos, forks));
	i = -1;
	while (++i < data->philos_nbr)
	{
		if (pthread_detach((philos + i)->philo) != 0)
			return (malloc_failure(data->philos_nbr, philos, forks));
	}
	data->start_time = get_current_time();
	pthread_mutex_lock(&data->start_routine_lock);
	data->start_routine = 1;
	pthread_mutex_unlock(&data->start_routine_lock);
	
	if (pthread_join(monitor, NULL) != 0)
			return (malloc_failure(data->philos_nbr, philos, forks));


	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->start_eating_lock);
	i = -1;
	while(++i < data->philos_nbr)
		pthread_mutex_destroy(&forks[i]);
	malloc_failure(data->philos_nbr, philos, forks);
	return (0);
}


int main(int ac, char **av)
{
	t_data	*data;
	int		i;

	data = malloc(sizeof(t_data));
	if (args_checker(ac, av, data))
		return (1);
	if (philo_init(data))
		return (1);
	i = -1;
	return (0);
}