/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 00:27:09 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/12 18:53:06 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <semaphore.h>
# include <sys/time.h>
# include <signal.h>
# include <limits.h>

# define SEM_FORKS "forks_sem"
# define SEM_HRT_BT "all_one_heart"

typedef struct s_shared
{
	int				no_philos;
	int				meals_per_philo;
	int				alive;
	int				hunger;
	int				forks;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			start_time;
	long			interval;
	struct timeval	start;
	struct timeval	sample;
	struct timeval	hngr_timer;
	sem_t			*forks_holder;
	sem_t			*all_heart_beating;
}	t_shared;

//main utilities
int		read_input_args(long *in_args, char **argv);
ssize_t	philo_atoi(const char *str, int *i);
void	fill_philo_args(t_shared *philo_args, long *in_args);

//Initiate and make processes as philosophers
void	launch_philosophers(t_shared *philo_args);

//within philosopher helper functions
void	randevu_dining_togather(t_shared *philo_args, int philo_tag);
void	time_since_start(t_shared	*philo_args);
long	time_since_ate(t_shared	*philo_args);
void	hold_status_vitality(t_shared *philo_args, int hold_time);
void	end_philosophers(int *philo_pids, int philos_count);

#endif
