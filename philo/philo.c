/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 00:58:39 by rboutaik          #+#    #+#             */
/*   Updated: 2024/06/06 23:00:44 by rboutaik         ###   ########.fr       */
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

int	_died(t_philo *philo)
{
	int	bol;

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

void	think(t_philo *philo)
{
	custom_print(philo, "is thinking\n");
}
void	eat(t_philo *philo)
{
	if (_died(philo))
		return ;
	pthread_mutex_lock(&philo->data->forks[philo->id]);
	if (_died(philo))
	{
		pthread_mutex_unlock(&philo->data->forks[philo->id]);
		return ;
	}
	custom_print(philo, "has taken a fork\n");
	
	if (philo->data->philos_nbr == 1)
		custom_usleep(philo->data->t_to_die, philo);
	if (_died(philo))
	{
		pthread_mutex_unlock(&philo->data->forks[philo->id]);
		return ;
	}
	pthread_mutex_lock(&philo->data->forks[(philo->id + 1) % philo->data->philos_nbr]);
	if (_died(philo))
	{
		pthread_mutex_unlock(&philo->data->forks[philo->id]);
		pthread_mutex_unlock(&philo->data->forks[(philo->id + 1) % philo->data->philos_nbr]);
		return ;
	}
	custom_print(philo, "has taken a fork\n");
	
	if (_died(philo))
	{
		pthread_mutex_unlock(&philo->data->forks[philo->id]);
		pthread_mutex_unlock(&philo->data->forks[(philo->id + 1) % philo->data->philos_nbr]);
		return ;
	}

	custom_print(philo, "is eating\n");

	pthread_mutex_lock(&philo->data->start_eating_lock);
	philo->start_eating = get_current_time();
	pthread_mutex_unlock(&philo->data->start_eating_lock);
	
	if (_died(philo))
	{
		pthread_mutex_unlock(&philo->data->forks[philo->id]);
		pthread_mutex_unlock(&philo->data->forks[(philo->id + 1) % philo->data->philos_nbr]);
		return ;
	}
		
	custom_usleep(philo->data->t_to_eat, philo);
	
	pthread_mutex_lock(&philo->data->meals_lock);
	philo->meals++;
	pthread_mutex_unlock(&philo->data->meals_lock);
	
	pthread_mutex_unlock(&philo->data->forks[philo->id]);
	pthread_mutex_unlock(&philo->data->forks[(philo->id + 1) % philo->data->philos_nbr]);
	
	
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
	int	meals;

	if (philo->id % 2 == 0)
		usleep(2000);
	
	while (1)
	{
		if (_died(philo))
			return (NULL);
		think(philo);
		eat(philo);
		_sleep(philo);
		
		pthread_mutex_lock(&philo->data->meals_lock);
		meals = philo->meals;
		pthread_mutex_unlock(&philo->data->meals_lock);
			
	}
	return (NULL);
}


void	*ft_monitor(t_philo *philos)
{
	int	i;
	int nb_philos;

	i = 0;
	nb_philos = philos[i].data->philos_nbr;
	while (1)
	{
		i = 0;
		while (i < nb_philos)
		{
			pthread_mutex_lock(&philos[i].data->start_eating_lock);
			if (get_current_time() - philos[i].start_eating > (size_t)philos[i].data->t_to_die)
			{
				pthread_mutex_unlock(&philos[i].data->start_eating_lock);
				pthread_mutex_lock(&philos[i].data->dead_lock);
				philos[i].data->is_dead = 1;
				pthread_mutex_unlock(&philos[i].data->dead_lock);
				
				pthread_mutex_lock(&philos[i].data->print);
				printf("%lu %d %s", get_current_time() - philos[i].data->start_time, i + 1, "died\n");
				pthread_mutex_unlock(&philos[i].data->print);
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
	if (pthread_mutex_init(&data->dead_lock, NULL) != 0)
		return (malloc_failure(i, philos, forks));
	if (pthread_mutex_init(&data->meals_lock, NULL) != 0)
		return (malloc_failure(i, philos, forks));
	data->forks = forks;
	i = -1;
	data->start_time = get_current_time();
	while (++i < data->philos_nbr)
	{
		(philos + i)->id = i;
		(philos + i)->data = data;
		(philos + i)->meals = 0;
		(philos + i)->start_eating = get_current_time();
		if (pthread_create(&(philos + i)->philo, NULL, (void *)routine, (void *)(philos + i)) != 0)
			return (malloc_failure(data->philos_nbr, philos, forks));
	}
	
	// usleep(1000);
	
	ft_monitor(philos);
	
	i = -1;
	while (++i < data->philos_nbr)
	{
		if (pthread_join((philos + i)->philo, NULL) != 0)
			return (malloc_failure(data->philos_nbr, philos, forks));
	}

	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->dead_lock);
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