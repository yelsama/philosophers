/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 17:33:51 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/17 20:28:07 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

long	time_since_start(t_shared	*philo_args)
{
	long	interv;

	gettimeofday(&philo_args->sample, NULL);
	interv = ((philo_args->sample.tv_sec
				* 1000000 + philo_args->sample.tv_usec)
			- philo_args->start_time);
	return (interv);
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

void	hold_status_vitality(t_shared *philo_args, long hold_time)
{
	long	start_time;
	long	time_left;
	long	time_to_sleep;

	start_time = time_since_start(philo_args);
	time_left = philo_args->time_to_die - time_since_ate(philo_args);
	if (hold_time < time_left)
		time_to_sleep = hold_time;
	else
		time_to_sleep = time_left;
	time_left = time_since_start(philo_args);
	while (time_left - start_time < time_to_sleep)
	{
		usleep(50);
		time_left = time_since_start(philo_args);
	}
}

void	end_philosophers(int *philo_pids, t_shared *philo_args)
{
	int	i;

	i = -1;
	while (++i < philo_args->no_philos)
		kill(philo_pids[i], SIGTERM);
}
