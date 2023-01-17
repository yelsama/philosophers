/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 00:27:09 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/17 20:44:40 by ymohamed         ###   ########.fr       */
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
# include <pthread.h>

# define SEM_FORKS "forks_sem"
# define SEM_HRT_BT "all_one_heart"
# define SEM_CLEAN_THRDS "clean_threads"

typedef enum e_events
{
	forks_pick = 1,
	eating = 2,
	sleeping = 3,
	thinking = 4,
}	t_evnet_msgs;

typedef struct s_shared
{
	int				no_philos;
	int				philo_tag;
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
	sem_t			*clean_threds;
	pthread_t		vitality;
	pthread_t		actions;
	pthread_t		clearing;
	pthread_mutex_t	lock_hunger;
	pthread_mutex_t	lock_vitality;
}	t_shared;

//main utilities
int		read_input_args(int *in_args, char **argv);
ssize_t	philo_atoi(const char *str, int *i);
void	fill_philo_args(t_shared *philo_args, int *in_args);
int		prepare_samphores(t_shared *philo_args);
void	close_semaphores(t_shared *philo_args);

//Initiate and make processes as philosophers
void	launch_philosophers(t_shared *philo_args);

//within philosopher helper functions
long	time_since_start(t_shared	*philo_args);
long	time_since_ate(t_shared	*philo_args);
void	hold_status_vitality(t_shared *philo_args, long hold_time);
void	end_philosophers(int *philo_pids, t_shared *philo_args);
void	print_action(t_shared *philo_args, int philo_tag, int msg);

//handle Threads for each philo
void	*vitality_hndlr(void *local);
void	*actions_hndlr(void *local);
void	*clear_philos_hndl(void *local);

//threads locked funcions
void	change_hunger_status(t_shared *philo_args);
void	change_vitality_status(t_shared *philo_args);
int		hungry(t_shared *philo_args);
int		alive(t_shared *philo_args);

#endif
