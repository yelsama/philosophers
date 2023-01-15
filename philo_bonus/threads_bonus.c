/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 23:58:37 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/16 00:15:42 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	print_eating_msgs(t_shared *philo_args, int philo_tag)
{
	printf("%lu philosopher %d has taken a fork\n",
		philo_args->interval / 1000, philo_tag);
	printf("%lu philosopher %d has taken a fork\n",
		philo_args->interval / 1000, philo_tag);
	printf("%lu philosopher %d is eating\n",
		philo_args->interval / 1000, philo_tag);
}

static void	philo_go_sleep_think(t_shared *philo_args, int philo_tag)
{
	time_since_start(philo_args);
	sem_wait(philo_args->all_heart_beating);
	printf("%lu philosopher %d is sleeping\n",
		philo_args->interval / 1000, philo_tag);
	sem_post(philo_args->all_heart_beating);
	hold_status_vitality(philo_args, philo_args->time_to_sleep);
	if (philo_args->alive)
	{
		time_since_start(philo_args);
		sem_wait(philo_args->all_heart_beating);
		philo_args->hunger = 1;
		printf("%lu philosopher %d is thinking\n",
			philo_args->interval / 1000, philo_tag);
		sem_post(philo_args->all_heart_beating);
	}
}

void	*vitality_hndlr(void *local)
{
	t_shared	*philo_args;

	philo_args = local;
	while (philo_args->alive)
	{
		if (philo_args->hunger)
		{
			hold_status_vitality(philo_args, philo_args->time_to_eat);
			if (!philo_args->alive)
				sem_post(philo_args->forks_holder);
		}
		else
			usleep(philo_args->time_to_sleep - 100);
	}
	return (NULL);
}

void	*actions_hndlr(void *local)
{
	t_shared	*philo_args;

	philo_args = local;
	while (philo_args->alive && philo_args->meals_per_philo)
	{
		sem_wait(philo_args->forks_holder);
		if (time_since_ate(philo_args) > philo_args->time_to_die)
			philo_args->alive = 0;
		if (philo_args->alive && philo_args->meals_per_philo
			&& philo_args->no_philos > 1)
		{
			gettimeofday(&philo_args->hngr_timer, NULL);
			time_since_start(philo_args);
			sem_wait(philo_args->all_heart_beating);
			print_eating_msgs(philo_args, philo_args->philo_tag);
			sem_post(philo_args->all_heart_beating);
			hold_status_vitality(philo_args, philo_args->time_to_eat);
			philo_args->hunger = 0;
			philo_args->meals_per_philo--;
		}
		sem_post(philo_args->forks_holder);
		if (philo_args->alive && !philo_args->hunger)
			philo_go_sleep_think(philo_args, philo_args->philo_tag);
	}
	return (NULL);
}
