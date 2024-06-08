/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 00:58:39 by rboutaik          #+#    #+#             */
/*   Updated: 2024/06/08 08:38:45 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_monitor(t_philo *philo)
{
	int	nb_philos;

	nb_philos = philo->data->philos_nbr;
	while (1)
	{
		sem_wait(philo->data->start_eating_lock);
		if (get_current_time()
			- philo->start_eating >= (size_t)philo->data->t_to_die)
		{
			return (philos_killer(philo, philo->id));
		}
		sem_post(philo->data->start_eating_lock);
	}
	return (NULL);
}

int	ft_destroy(int mut, t_data *data)
{
	sem_close(data->forks);
	sem_unlink("forks_sem");
	if (mut >= 1)
	{
		sem_close(data->start_eating_lock);
		sem_unlink("start_eating_sem");
	}
	if (mut >= 2)
	{
		sem_close(data->print);
		sem_unlink("print_sem");
	}
	if (mut >= 4)
	{
		sem_close(data->meals_lock);
		sem_unlink("meals_sem");
	}
	return (1);
}

int	semaphore_init(t_data *data, t_philo *philos)
{
	sem_unlink("forks_sem");
	data->forks = sem_open("forks_sem", O_CREAT, 0600, data->philos_nbr);
	if (data->forks == SEM_FAILED)
		return (ft_destroy(0, data), ft_free(philos));
	sem_unlink("start_eating_sem");
	data->start_eating_lock = sem_open("start_eating_sem", O_CREAT, 0600, 1);
	if (data->start_eating_lock == SEM_FAILED)
		return (ft_destroy(0, data), ft_free(philos));
	sem_unlink("print_sem");
	data->print = sem_open("print_sem", O_CREAT, 0600, 1);
	if (data->print == SEM_FAILED)
		return (ft_destroy(1, data), ft_free(philos));
	sem_unlink("meals_sem");
	data->meals_lock = sem_open("meals_sem", O_CREAT, 0600, 1);
	if (data->meals_lock == SEM_FAILED)
		return (ft_destroy(3, data), ft_free(philos));
	data->start_time = get_current_time();
	return (0);
}

int	philo_init(t_data *data)
{
	t_philo			*philos;
	int				i;
	int				status;

	philos = malloc(sizeof(t_philo) * (data->philos_nbr));
	if (philos == NULL || semaphore_init(data, philos))
		return (1);
	i = -1;
	while (++i < data->philos_nbr)
		if (create_philos(philos, i, data))
			return (1);
	waitpid(-1, &status, 0);
	if (WEXITSTATUS(status) == 1)
	{
		i = -1;
		while (++i < data->philos_nbr)
			kill(philos[i].philo_pid, SIGKILL);
	}
	else if (WEXITSTATUS(status) == 2)
		(ft_destroy(5, data), ft_free(philos), free(data), exit(2));
	else
		while (waitpid(-1, NULL, 0) != -1)
			;
	return (ft_destroy(5, data), ft_free(philos), 0);
}

int	main(int ac, char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (args_checker(ac, av, data))
	{
		if (data->philos_nbr == 0 || data->eat_nbr == 0)
			return (free(data), 0);
		return (free(data), 1);
	}
	if (philo_init(data))
		return (free(data), 1);
	free(data);
	return (0);
}
