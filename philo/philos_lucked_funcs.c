/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_lucked_funcs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 00:23:53 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/09 11:41:15 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	all_alive(t_philosophers *ph_info)
{
	int	ret;

	pthread_mutex_lock(&ph_info->resrc->g_lock);
	ret = ph_info->resrc->all_alive;
	pthread_mutex_unlock(&ph_info->resrc->g_lock);
	return (ret);
}

void	print_action(t_philosophers *ph_info, t_in_thread_vars *local, int msg)
{
	pthread_mutex_lock(&ph_info->resrc->g_lock);
	if (msg == death && ph_info->resrc->all_alive)
	{
		printf("%ld Philosopher %d died\n", local->interval / 1000,
			ph_info->philo_tag + 1);
		ph_info->resrc->all_alive = 0;
	}
	if (!ph_info->resrc->all_alive && !pthread_mutex_unlock
		(&ph_info->resrc->g_lock))
		return ;
	if (msg == forks_pick)
		printf("%ld philosopher %d has taken a fork\n",
			local->interval / 1000, ph_info->philo_tag + 1);
	time_since_start(local);
	if (msg == eating)
		printf("%ld philosopher %d is eating\n", local->interval / 1000,
			ph_info->philo_tag + 1);
	if (msg == sleeping)
		printf("%ld philosopher %d is sleeping\n",
			local->interval / 1000, ph_info->philo_tag + 1);
	if (msg == thinking)
		printf("%ld philosopher %d is thinking\n", local->interval / 1000,
			ph_info->philo_tag + 1);
	pthread_mutex_unlock(&ph_info->resrc->g_lock);
}

int	check_quta(t_philosophers *ph_info)
{
	int	ret;

	pthread_mutex_lock(&ph_info->resrc->g_lock);
	ret = ph_info->resrc->quta_give_to;
	pthread_mutex_unlock(&ph_info->resrc->g_lock);
	return (ret);
}

int	check_forks(t_in_thread_vars *local, t_philosophers *ph_info)
{
	time_since_start(local);
	pthread_mutex_lock(&ph_info->resrc->locks[local->philo_tag]);
	if (ph_info->resrc->forks[local->fork_left] < 0
		|| ph_info->resrc->forks[local->fork_right] < 0
		|| ph_info->resrc->forks[local->fork_left] == local->philo_tag
		|| ph_info->resrc->forks[local->fork_right] == local->philo_tag)
	{
		pthread_mutex_unlock(&ph_info->resrc->locks[local->philo_tag]);
		return (0);
	}
	ph_info->resrc->forks[local->fork_left] = -1;
	ph_info->resrc->forks[local->fork_right] = -1;
	print_action(ph_info, local, forks_pick);
	print_action(ph_info, local, forks_pick);
	pthread_mutex_unlock(&ph_info->resrc->locks[local->philo_tag]);
	return (1);
}

void	fill_thred_local_vars(t_in_thread_vars *local, t_philosophers *ph_info)
{
	local->interval = 0;
	local->fork_left = ph_info->philo_tag;
	local->fork_right = ph_info->philo_tag + 1;
	pthread_mutex_lock(&ph_info->resrc->g_lock);
	if (ph_info->philo_tag == ph_info->resrc->no_philos - 1)
		local->fork_right = 0;
	local->meals = ph_info->resrc->meals_per_philo;
	local->start_time = ph_info->resrc->start_time;
	local->no_of_philos = ph_info->resrc->no_philos;
	local->time_to_die = ph_info->resrc->time_to_die;
	local->time_to_sleep = ph_info->resrc->time_to_sleep;
	local->time_to_eat = ph_info->resrc->time_to_eat;
	pthread_mutex_unlock(&ph_info->resrc->g_lock);
	local->philo_tag = ph_info->philo_tag;
	local->left_neibr_lock = local->philo_tag -1;
	if (local->left_neibr_lock < 0)
		local->left_neibr_lock = local->no_of_philos - 1;
	local->vitality = 1;
	local->hunger = 1;
	local->avoid_lock_time = 0;
}
