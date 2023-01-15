/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 00:40:38 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/15 23:18:05 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static int	create_philosophers(int *in_args)
{
	t_shared		philo_args;
	int				forks_pair;

	forks_pair = in_args[0] / 2;
	if (forks_pair == 0)
		forks_pair = 1;
	fill_philo_args(&philo_args, in_args);
	sem_unlink(SEM_HRT_BT);
	sem_unlink(SEM_FORKS);
	philo_args.all_heart_beating
		= sem_open(SEM_HRT_BT, O_CREAT, 0660, 1);
	if (philo_args.all_heart_beating == SEM_FAILED)
		return (write(2, "Semaphore Error\n", 17), 0);
	philo_args.forks_holder
		= sem_open(SEM_FORKS, O_CREAT, 0660, forks_pair);
	if (philo_args.forks_holder == SEM_FAILED)
		return (write(2, "Semaphore Error\n", 17),
			sem_close(philo_args.all_heart_beating), sem_unlink(SEM_HRT_BT), 0);
	launch_philosophers(&philo_args);
	sem_close(philo_args.forks_holder);
	sem_unlink(SEM_FORKS);
	sem_close(philo_args.all_heart_beating);
	sem_unlink(SEM_HRT_BT);
	return (1);
}

int	main(int argc, char **argv)
{
	int	in_args[5];
	int	i;

	i = -1;
	while (++i < 5)
		in_args[i] = -1;
	if (argc < 5 || argc > 6)
		return (write(2, "Error, number of arguments\n", 27), 1);
	if (!read_input_args(in_args, argv))
		return (write(2, "Error, Arguments not suitable\n", 30), 1);
	if (!create_philosophers(in_args))
		return (write(2, "Error Making Process\n", 21), 1);
	return (0);
}
