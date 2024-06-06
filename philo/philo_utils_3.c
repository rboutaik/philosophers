/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 00:20:49 by rboutaik          #+#    #+#             */
/*   Updated: 2024/06/07 00:28:50 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philos(t_philo *philos, int i, pthread_mutex_t *forks, t_data *data)
{
	(philos + i)->id = i;
	(philos + i)->data = data;
	(philos + i)->meals = 0;
	(philos + i)->start_eating = get_current_time();
	if (pthread_create(&(philos + i)->philo, NULL,
			(void *)routine, (void *)(philos + i)) != 0)
		return (ft_destroy(forks, i, 5, data), ft_free(philos, forks));
	return (0);
}
