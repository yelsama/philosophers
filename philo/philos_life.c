/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_life.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 04:13:30 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/09 12:07:10 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	go_sleeping(t_philosophers *ph_info, t_in_thread_vars	*local)
{
	time_since_start(local);
	print_action(ph_info, local, sleeping);
	hold_status_vitality(local, local->time_to_sleep);
	if (!local->vitality)
		return ;
	local->hunger = 1;
}

static void	go_eating(t_philosophers *ph_info, t_in_thread_vars	*local)
{
	gettimeofday(&local->hngr_timer, NULL);
	time_since_start(local);
	print_action(ph_info, local, eating);
	hold_status_vitality(local, local->time_to_eat);
	if (!local->vitality)
		return ;
	pthread_mutex_lock(&ph_info->resrc->locks[local->fork_right]);
	ph_info->resrc->forks[local->fork_right] = local->philo_tag;
	pthread_mutex_unlock(&ph_info->resrc->locks[local->fork_right]);
	pthread_mutex_lock(&ph_info->resrc->locks[local->left_neibr_lock]);
	ph_info->resrc->forks[local->fork_left] = local->philo_tag;
	pthread_mutex_unlock(&ph_info->resrc->locks[local->left_neibr_lock]);
	local->hunger = 0;
	local->meals--;
	if (local->meals == 0 && check_quta(ph_info))
	{
		pthread_mutex_lock(&ph_info->resrc->g_lock);
		ph_info->resrc->quta_give_to--;
		pthread_mutex_unlock(&ph_info->resrc->g_lock);
	}
}

static void	philo_life_cycle(t_philosophers *ph_info, t_in_thread_vars *local)
{
	while (all_alive(ph_info) && check_quta(ph_info) && local->vitality)
	{
		while (local->hunger && local->vitality)
		{
			if (all_alive(ph_info) && local->no_of_philos > 1
				&& check_forks(local, ph_info))
				go_eating(ph_info, local);
			if (time_since_ate(local) > local->time_to_die)
				local->vitality = 0;
			avoid_deadlock_on_forks(local);
		}
		if (!local->hunger && local->vitality && all_alive(ph_info))
			go_sleeping(ph_info, local);
		if (all_alive(ph_info) && local->vitality)
		{
			time_since_start(local);
			print_action(ph_info, local, thinking);
		}
	}
}

void	*thrd_hndlr(void *philo)
{
	t_philosophers		*ph_info;
	t_in_thread_vars	local;

	ph_info = philo;
	fill_thred_local_vars(&local, ph_info);
	randevu_dining_togather(&local);
	philo_life_cycle(ph_info, &local);
	if (!local.vitality && all_alive(ph_info))
	{
		time_since_start(&local);
		print_action(ph_info, &local, death);
	}
	return (NULL);
}
