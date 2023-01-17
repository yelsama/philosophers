/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 17:25:33 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/17 20:26:43 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	clear_all_philosophers_locks(t_shared *philo_args)
{
	int	i;

	i = -1;
	while (++i < philo_args->no_philos)
	{
		sem_post(philo_args->clean_threds);
		usleep(50);
	}
}

static void	philo_actions(t_shared *philo_args, int philo_tag)
{
	philo_args->philo_tag = philo_tag;
	pthread_create(&philo_args->vitality, NULL,
		&vitality_hndlr, philo_args);
	pthread_create(&philo_args->actions, NULL,
		&actions_hndlr, philo_args);
	pthread_create(&philo_args->clearing, NULL,
		&clear_philos_hndl, philo_args);
	pthread_join(philo_args->vitality, NULL);
}

static void	philo_life_cycle(t_shared *philo_args, int philo_tag)
{
	long	passed_time;

	gettimeofday(&philo_args->hngr_timer, NULL);
	philo_actions(philo_args, philo_tag);
	if (!philo_args->alive && philo_args->meals_per_philo)
	{
		passed_time = time_since_start(philo_args) / 1000;
		sem_wait(philo_args->all_heart_beating);
		printf("%lu philosopher %d died\n", passed_time, philo_tag);
		clear_all_philosophers_locks(philo_args);
		pthread_join(philo_args->clearing, NULL);
		exit(1);
	}
	pthread_join(philo_args->actions, NULL);
	pthread_mutex_destroy(&philo_args->lock_vitality);
	pthread_mutex_destroy(&philo_args->lock_hunger);
	exit (0);
}

static void	prepare_child_local_args(t_shared *philo_args, int *children_pids)
{
	free(children_pids);
	pthread_mutex_init(&philo_args->lock_hunger, NULL);
	pthread_mutex_init(&philo_args->lock_vitality, NULL);
}

void	launch_philosophers(t_shared *philo_args)
{
	int		i;
	int		pid;
	int		*children_pids;
	int		status;

	i = -1;
	pid = 1;
	children_pids = malloc(sizeof(int) * philo_args->no_philos);
	if (!children_pids && write(2, "Error mallocing\n", 16))
		return ;
	while (++i < philo_args->no_philos && pid)
	{
		pid = fork();
		children_pids[i] = pid;
		if (!pid)
		{
			prepare_child_local_args(philo_args, children_pids);
			philo_life_cycle(philo_args, i + 1);
		}
	}
	while (waitpid(-1, &status, 0) > 0)
		if (status > 0)
			end_philosophers(children_pids, philo_args);
	free(children_pids);
}	
