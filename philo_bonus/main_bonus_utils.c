/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 12:36:53 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/17 20:44:06 by ymohamed         ###   ########.fr       */
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
			+ start.tv_usec);
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
	return (1);
}

int	prepare_samphores(t_shared *philo_args)
{
	int				forks_pair;

	forks_pair = philo_args->no_philos / 2;
	if (forks_pair == 0)
		forks_pair = 1;
	sem_unlink(SEM_HRT_BT);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_CLEAN_THRDS);
	philo_args->all_heart_beating
		= sem_open(SEM_HRT_BT, O_CREAT, 0660, 1);
	if (philo_args->all_heart_beating == SEM_FAILED)
		return (0);
	philo_args->forks_holder
		= sem_open(SEM_FORKS, O_CREAT, 0660, forks_pair);
	if (philo_args->forks_holder == SEM_FAILED)
		return (sem_close(philo_args->all_heart_beating),
			sem_unlink(SEM_HRT_BT), 0);
	philo_args->clean_threds
		= sem_open(SEM_CLEAN_THRDS, O_CREAT, 0660, 0);
	if (philo_args->clean_threds == SEM_FAILED)
		return (sem_close(philo_args->all_heart_beating),
			sem_unlink(SEM_HRT_BT), sem_close(philo_args->forks_holder),
			sem_unlink(SEM_FORKS), 0);
	return (1);
}

void	close_semaphores(t_shared *philo_args)
{
	sem_close(philo_args->clean_threds);
	sem_unlink(SEM_CLEAN_THRDS);
	sem_close(philo_args->forks_holder);
	sem_unlink(SEM_FORKS);
	sem_close(philo_args->all_heart_beating);
	sem_unlink(SEM_HRT_BT);
}
