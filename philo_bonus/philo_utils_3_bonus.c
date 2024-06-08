/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_3_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 00:20:49 by rboutaik          #+#    #+#             */
/*   Updated: 2024/06/08 08:39:05 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	create_philos(t_philo *philos, int i, t_data *data)
{
	(philos + i)->id = i;
	(philos + i)->data = data;
	(philos + i)->meals = 0;
	(philos + i)->start_eating = get_current_time();
	philos[i].philo_pid = fork();
	if (philos[i].philo_pid < 0)
	{
		(ft_destroy(5, data), ft_free(philos));
		free(data);
		exit(1);
	}
	if (philos[i].philo_pid == 0)
		routine(philos + i);
	return (0);
}
