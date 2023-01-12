/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_life_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 04:26:22 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/09 23:45:44 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	randevu_dining_togather(t_in_thread_vars	*local)
{
	long	randv_time;
	long	time_passed;

	randv_time = local->start_time;
	time_passed = 0;
	while (time_passed < randv_time)
	{
		usleep(50);
		gettimeofday(&local->sample, NULL);
		time_passed = (local->sample.tv_sec * 1000000 + local->sample.tv_usec);
	}
	gettimeofday(&local->hngr_timer, NULL);
	if (local->philo_tag % 2 == 1)
		hold_status_vitality(local, local->time_to_eat - 10);
}

void	avoid_deadlock_on_forks(t_in_thread_vars *local)
{
	local->avoid_lock_time += 5;
	usleep(local->avoid_lock_time);
	if (local->avoid_lock_time > 2 * local->no_of_philos)
		local->avoid_lock_time = 0;
}

long	time_since_ate(t_in_thread_vars	*local)
{
	long	intrv;

	gettimeofday(&local->sample, NULL);
	intrv = ((local->sample.tv_sec * 1000000 + local->sample.tv_usec)
			- (local->hngr_timer.tv_sec * 1000000 + local->hngr_timer.tv_usec));
	return (intrv);
}

void	time_since_start(t_in_thread_vars	*local)
{
	gettimeofday(&local->sample, NULL);
	local->interval = ((local->sample.tv_sec * 1000000 + local->sample.tv_usec)
			- local->start_time);
}

void	hold_status_vitality(t_in_thread_vars *local, long hold_time)
{
	long	start_time;
	long	time_left;
	long	time_to_sleep;

	time_since_start(local);
	start_time = local->interval;
	time_left = local->time_to_die - time_since_ate(local);
	if (hold_time < time_left)
		time_to_sleep = hold_time;
	else
	{
		time_to_sleep = time_left;
		local->vitality = 0;
	}
	while (local->interval - start_time < time_to_sleep)
	{
		usleep(50);
		time_since_start(local);
	}
}
