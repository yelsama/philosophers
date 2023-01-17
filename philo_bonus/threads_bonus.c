/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 23:58:37 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/17 20:45:42 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	print_action(t_shared *philo_args, int philo_tag, int msg)
{
	long	passed_time;

	passed_time = time_since_start(philo_args) / 1000;
	sem_wait(philo_args->all_heart_beating);
	if (msg == forks_pick)
		printf("%ld philosopher %d has taken a fork\n", passed_time, philo_tag);
	if (msg == eating)
		printf("%ld philosopher %d is eating\n", passed_time, philo_tag);
	if (msg == sleeping)
		printf("%ld philosopher %d is sleeping\n", passed_time, philo_tag);
	if (msg == thinking)
		printf("%ld philosopher %d is thinking\n", passed_time, philo_tag);
	sem_post(philo_args->all_heart_beating);
}

static void	philo_go_sleep_think(t_shared *philo_args, int philo_tag)
{
	print_action(philo_args, philo_tag, sleeping);
	hold_status_vitality(philo_args, philo_args->time_to_sleep);
	if (alive(philo_args))
	{
		time_since_start(philo_args);
		change_hunger_status(philo_args);
		print_action(philo_args, philo_args->philo_tag, thinking);
	}
}

void	*vitality_hndlr(void *local)
{
	t_shared	*philo_args;
	long		left_time;

	philo_args = local;
	left_time = philo_args->time_to_die - philo_args->time_to_sleep;
	while (philo_args->alive)
	{
		if (hungry(philo_args))
			hold_status_vitality(philo_args, left_time);
		else
			usleep(100);
		if (time_since_ate(philo_args) > philo_args->time_to_die)
			change_vitality_status(philo_args);
	}
	return (NULL);
}

void	*actions_hndlr(void *local)
{
	t_shared	*philo_args;

	philo_args = local;
	while (alive(philo_args) && philo_args->meals_per_philo)
	{
		sem_wait(philo_args->forks_holder);
		if (time_since_ate(philo_args) > philo_args->time_to_die)
			change_vitality_status(philo_args);
		else
		{
			print_action(philo_args, philo_args->philo_tag, forks_pick);
			print_action(philo_args, philo_args->philo_tag, forks_pick);
			gettimeofday(&philo_args->hngr_timer, NULL);
			change_hunger_status(philo_args);
			print_action(philo_args, philo_args->philo_tag, eating);
			philo_args->meals_per_philo--;
		}
		hold_status_vitality(philo_args, philo_args->time_to_eat);
		sem_post(philo_args->forks_holder);
		if (alive(philo_args) && !hungry(philo_args))
			philo_go_sleep_think(philo_args, philo_args->philo_tag);
	}
	return (NULL);
}

void	*clear_philos_hndl(void *local)
{
	t_shared	*philo_args;

	philo_args = local;
	sem_wait(philo_args->clean_threds);
	pthread_mutex_destroy(&philo_args->lock_vitality);
	pthread_mutex_destroy(&philo_args->lock_hunger);
	return (NULL);
}
