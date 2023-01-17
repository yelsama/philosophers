/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locked_funcs_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 16:23:44 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/17 19:03:47 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	change_hunger_status(t_shared *philo_args)
{
	pthread_mutex_lock(&philo_args->lock_hunger);
	if (philo_args->hunger)
		philo_args->hunger = 0;
	else
		philo_args->hunger = 1;
	pthread_mutex_unlock(&philo_args->lock_hunger);
}

void	change_vitality_status(t_shared *philo_args)
{
	pthread_mutex_lock(&philo_args->lock_vitality);
	philo_args->alive = 0;
	pthread_mutex_unlock(&philo_args->lock_vitality);
}

int	hungry(t_shared *philo_args)
{
	int	i;

	pthread_mutex_lock(&philo_args->lock_hunger);
	i = philo_args->hunger;
	pthread_mutex_unlock(&philo_args->lock_hunger);
	return (i);
}

int	alive(t_shared *philo_args)
{
	int	i;

	pthread_mutex_lock(&philo_args->lock_vitality);
	i = philo_args->alive;
	pthread_mutex_unlock(&philo_args->lock_vitality);
	return (i);
}
