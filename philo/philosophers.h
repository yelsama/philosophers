/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 00:27:09 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/09 23:43:36 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <sys/time.h>

typedef enum e_events
{
	forks_pick = 1,
	eating = 2,
	sleeping = 3,
	thinking = 4,
	death = 5,
}	t_evnet_msgs;

typedef struct s_shared
{
	int				no_philos;
	int				meals_per_philo;
	int				quta_give_to;
	int				*forks;
	int				all_alive;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			start_time;
	pthread_mutex_t	*locks;
	pthread_mutex_t	g_lock;
	struct timeval	start;
}	t_shared;

typedef struct s_philosophers
{
	int				philo_tag;
	pthread_t		philo_thred;
	t_shared		*resrc;
}	t_philosophers;

typedef struct s_inthread
{
	int				philo_tag;
	int				meals;
	int				fork_left;
	int				fork_right;
	int				hunger;
	int				vitality;
	int				no_of_philos;
	int				avoid_lock_time;
	int				left_neibr_lock;
	long			time_to_die;
	long			time_to_sleep;
	long			time_to_eat;
	long			start_time;
	long			interval;
	struct timeval	sample;
	struct timeval	hngr_timer;
}	t_in_thread_vars;

//main utilities
int				read_input_args(long *in_args, char **argv);
ssize_t			philo_atoi(const char *str, int *i);
t_philosophers	*intiate_thred_resources(t_shared *resrs, long *in_args);
void			clear_philos(int n, t_philosophers *philos, t_shared *resrs);

// thread handler
void			*thrd_hndlr(void *phil);

//thread handler helper functions
void			fill_thred_local_vars(t_in_thread_vars *local,
					t_philosophers *ph_info);
void			randevu_dining_togather(t_in_thread_vars *local);
void			avoid_deadlock_on_forks(t_in_thread_vars *local);
long			time_since_ate(t_in_thread_vars	*local);
void			time_since_start(t_in_thread_vars	*local);
void			hold_status_vitality(t_in_thread_vars *local, long hold_time);

//thread handler lucked funtions
int				check_forks(t_in_thread_vars *local,
					t_philosophers *philos_info);
int				all_alive(t_philosophers *ph_info);
int				check_quta(t_philosophers *ph_info);
void			print_action(t_philosophers *ph_info,
					t_in_thread_vars *local, int msg);
#endif
