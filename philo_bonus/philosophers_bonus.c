/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 17:25:33 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/10 17:43:27 by ymohamed         ###   ########.fr       */
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
		printf("%lu philosopher %d is thinking\n",
			philo_args->interval / 1000, philo_tag);
		sem_post(philo_args->all_heart_beating);
	}
}

static void	philo_actions(t_shared *philo_args, int philo_tag)
{
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
			print_eating_msgs(philo_args, philo_tag);
			sem_post(philo_args->all_heart_beating);
			hold_status_vitality(philo_args, philo_args->time_to_eat);
			philo_args->hunger = 0;
			philo_args->meals_per_philo--;
		}
		sem_post(philo_args->forks_holder);
		if (philo_args->alive && !philo_args->hunger)
			philo_go_sleep_think(philo_args, philo_tag);
	}
}

static void	philo_life_cycle(t_shared *philo_args, int philo_tag)
{
	randevu_dining_togather(philo_args, philo_tag);
	philo_actions(philo_args, philo_tag);
	if (!philo_args->alive)
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
