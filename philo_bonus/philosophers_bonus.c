/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 17:25:33 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/15 23:25:25 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	philo_actions(t_shared *philo_args, int philo_tag)
{
	philo_args->philo_tag = philo_tag;
	pthread_create(&philo_args->vitality, NULL,
		&vitality_hndlr, philo_args);
	pthread_create(&philo_args->actions, NULL,
		&actions_hndlr, philo_args);
	pthread_join(philo_args->vitality, NULL);
	pthread_join(philo_args->vitality, NULL);
}

static void	philo_life_cycle(t_shared *philo_args, int philo_tag)
{
	randevu_dining_togather(philo_args, philo_tag);
	philo_actions(philo_args, philo_tag);
	if (!philo_args->alive && philo_args->meals_per_philo)
	{
		time_since_start(philo_args);
		sem_wait(philo_args->all_heart_beating);
		printf("%lu philosopher %d died\n",
			philo_args->interval / 1000, philo_tag);
		exit(1);
	}
	exit (0);
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
		usleep(100);
	}
	if (!pid)
	{
		free(children_pids);
		philo_life_cycle(philo_args, i);
	}
	while (waitpid(-1, &status, 0) > 0)
		if (status > 0)
			end_philosophers(children_pids, philo_args->no_philos);
	free(children_pids);
}	
