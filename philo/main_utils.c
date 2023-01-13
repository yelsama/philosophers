/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 12:36:53 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/09 23:37:45 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	clear_philos(int n, t_philosophers *philos, t_shared *resrs)
{
	int	i;

	i = -1;
	while (++i < n)
		pthread_join(philos[i].philo_thred, NULL);
	i = -1;
	while (++i < resrs->no_philos)
		pthread_mutex_destroy(&resrs->locks[i]);
	pthread_mutex_destroy(&resrs->g_lock);
	free(resrs->locks);
	free(philos);
	free(resrs->forks);
}

static void	assign_forks(int *forks, int no_philos)
{
	int	i;
	int	odd;

	i = -1;
	odd = 1;
	while (++i < no_philos)
	{
		forks[i] = odd;
		if (i == odd)
			odd += 2;
	}
}

static int	create_locks_for_threds(t_shared *resrs)
{
	int	i;
	int	n;

	i = -1;
	n = -1;
	resrs->locks = malloc(sizeof(pthread_mutex_t) * resrs->no_philos);
	if (!resrs->locks)
		return (0);
	while (++i < resrs->no_philos)
	{
		if (pthread_mutex_init(&resrs->locks[i], NULL))
		{
			while (++n < i)
				pthread_mutex_destroy(&resrs->locks[n]);
			return (0);
		}
	}
	return (1);
}

t_philosophers	*intiate_thred_resources(t_shared *resrs, long *in_args)
{
	t_philosophers	*philo_threds;

	resrs->all_alive = 1;
	resrs->no_philos = in_args[0];
	resrs->time_to_die = in_args[1] * 1000;
	resrs->time_to_eat = in_args[2] * 1000;
	resrs->time_to_sleep = in_args[3] * 1000;
	resrs->meals_per_philo = in_args[4];
	resrs->quta_give_to = in_args[0];
	if (pthread_mutex_init(&resrs->g_lock, NULL))
		return (0);
	resrs->forks = malloc(sizeof(int) * resrs->no_philos);
	if (!resrs->forks)
		return (NULL);
	if (!create_locks_for_threds(resrs))
		return (free(resrs->forks), NULL);
	philo_threds = malloc(sizeof(t_philosophers) * in_args[0]);
	if (!philo_threds)
		return (free(resrs->forks), free(resrs->locks), NULL);
	assign_forks(resrs->forks, resrs->no_philos);
	return (philo_threds);
}

int	read_input_args(long *in_args, char **argv)
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
