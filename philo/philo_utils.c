/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 10:08:22 by rboutaik          #+#    #+#             */
/*   Updated: 2024/06/06 23:37:31 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int				i;
	int				signe;
	long			nb;

	i = 0;
	signe = 1;
	nb = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i++] == '-')
			signe *= -1;
	}
	while (str[i] && str[i] >= 48 && str[i] <= 57)
	{
		nb = nb * 10 + str[i++] - 48;
		if (nb > INT_MAX && signe == 1)
			return (-1);
		else if (nb > (long)INT_MAX + 1 && signe == -1)
			return (-1);
	}
	if (str[i])
		return (-1);
	return (nb * signe);
}

int	args_failure(int flag)
{
	if (flag == 1)
		printf("Wrong number of args :(\n");
	else if (flag == 2)
		printf("Wrong args try with valid ones :(\n");
	return (1);
}

int	args_checker(int ac, char **av, t_data *data)
{
	if (ac != 5 && ac != 6)
		return (args_failure(1));
	data->philos_nbr = ft_atoi(av[1]);
	data->t_to_die = ft_atoi(av[2]);
	data->t_to_eat = ft_atoi(av[3]);
	data->t_to_sleep = ft_atoi(av[4]);
	if (ac == 5)
		data->eat_nbr = -1;
	if (ac == 6)
		data->eat_nbr = ft_atoi(av[5]);
	if (data->philos_nbr < 0 || data->t_to_die < 0
		|| data->t_to_eat < 0 || data->t_to_sleep < 0)
		return (args_failure(2));
	if (ac == 6 && data->eat_nbr < 0)
		return (args_failure(2));
	if (data->philos_nbr == 0)
		return (1);
	return (0);
}

int	ft_free(t_philo *philos, pthread_mutex_t *forks)
{
	free(philos);
	philos = NULL;
	free(forks);
	forks = NULL;
	return (1);
}

void	custom_print(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->print);
	if (_died(philo))
	{
		pthread_mutex_unlock(&philo->data->print);
		return ;
	}
	printf("%lu %d %s",
		get_current_time() - philo->data->start_time, philo->id + 1, str);
	pthread_mutex_unlock(&philo->data->print);
}
