/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 17:33:51 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/10 17:48:28 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	randevu_dining_togather(t_shared *philo_args, int philo_tag)
{
	long	randv_time;
	long	time_passed;

	randv_time = philo_args->start_time;
	time_passed = 0;
	while (time_passed < randv_time)
	{
		usleep(50);
		gettimeofday(&philo_args->sample, NULL);
		time_passed = (philo_args->sample.tv_sec * 1000000
				+ philo_args->sample.tv_usec);
	}
	gettimeofday(&philo_args->hngr_timer, NULL);
	if (philo_tag % 2 == 0)
		hold_status_vitality(philo_args, philo_args->time_to_eat);
}

void	time_since_start(t_shared	*philo_args)
{
	gettimeofday(&philo_args->sample, NULL);
	philo_args->interval = ((philo_args->sample.tv_sec
				* 1000000 + philo_args->sample.tv_usec)
			- philo_args->start_time);
}

long	time_since_ate(t_shared	*philo_args)
{
	long	intrv;

	gettimeofday(&philo_args->sample, NULL);
	intrv = ((philo_args->sample.tv_sec * 1000000 + philo_args->sample.tv_usec)
			- (philo_args->hngr_timer.tv_sec
				* 1000000 + philo_args->hngr_timer.tv_usec));
	return (intrv);
}

void	hold_status_vitality(t_shared *philo_args, int hold_time)
{
	long	start_time;
	int		time_left;
	long	time_to_sleep;

	time_since_start(philo_args);
	start_time = philo_args->interval;
	time_left = philo_args->time_to_die - time_since_ate(philo_args);
	if (hold_time < time_left)
		time_to_sleep = hold_time;
	else
	{
		time_to_sleep = time_left;
		philo_args->alive = 0;
	}
	while (philo_args->interval - start_time < time_to_sleep)
	{
		usleep(50);
		time_since_start(philo_args);
	}
}

void	end_philosophers(int *philo_pids, int philos_count)
{
	int	i;

	i = -1;
	while (++i < philos_count)
		kill(philo_pids[i], SIGTERM);
}
