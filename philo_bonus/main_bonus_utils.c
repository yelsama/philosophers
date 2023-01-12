/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 12:36:53 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/09 23:29:16 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	fill_philo_args(t_shared *philo_args, int *in_args)
{
	struct timeval	start;

	philo_args->no_philos = in_args[0];
	philo_args->time_to_die = in_args[1] * 1000;
	philo_args->time_to_eat = in_args[2] * 1000;
	philo_args->time_to_sleep = in_args[3] * 1000;
	philo_args->meals_per_philo = in_args[4];
	philo_args->alive = 1;
	philo_args->hunger = 1;
	gettimeofday(&start, NULL);
	philo_args->start_time = (start.tv_sec * 1000000
			+ start.tv_usec) + (500 * philo_args->no_philos);
}

int	read_input_args(int *in_args, char **argv)
{
	int	i;
	int	n;

	i = 0;
	while (argv[++i])
	{
		n = 0;
		in_args[i - 1] = philo_atoi(argv[i], &n);
		if (in_args[i - 1] < 1)
			return (0);
	}
	if (i == 5)
		in_args[i] = -1;
	return (1);
}
