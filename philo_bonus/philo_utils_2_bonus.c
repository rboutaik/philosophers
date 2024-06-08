/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_2_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 00:09:52 by rboutaik          #+#    #+#             */
/*   Updated: 2024/06/08 08:23:13 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday fialed\n", 21);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	custom_usleep(size_t milliseconds, t_philo *philo)
{
	size_t	start;

	(void)philo;
	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep (100);
	return (0);
}

void	*philos_killer(t_philo *philos, int i)
{
	sem_post(philos->data->start_eating_lock);
	sem_wait(philos->data->print);
	printf("%lu %d %s", get_current_time()
		- philos->data->start_time, i + 1, "died\n");
	exit(1);
}
