/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 00:40:38 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/05 10:34:36 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	launch_threds(t_shared *resrc, t_philosophers *philo_threds)
{
	int	i;

	i = -1;
	gettimeofday(&resrc->start, NULL);
	resrc->start_time = (resrc->start.tv_sec * 1000000
			+ resrc->start.tv_usec) + (500 * resrc->no_philos);
	while (++i < resrc->no_philos)
	{
		philo_threds[i].philo_tag = i;
		philo_threds[i].resrc = resrc;
		if (pthread_create(&philo_threds[i].philo_thred, NULL,
				&thrd_hndlr, &philo_threds[i]))
			return (clear_philos(i, philo_threds, resrc), 0);
	}
	return (1);
}

static int	create_philosophers(int *in_args)
{
	t_philosophers	*philo_threds;
	t_shared		resrc;

	if (in_args[0] < 1)
		return (0);
	philo_threds = intiate_thred_resources(&resrc, in_args);
	if (!philo_threds)
		return (0);
	if (launch_threds(&resrc, philo_threds))
		return (clear_philos(in_args[0], philo_threds, &resrc), 1);
	return (0);
}

int	main(int argc, char **argv)
{
	int	in_args[5];

	if (argc < 5 || argc > 6)
		return (write(2, "Error, number of arguments\n", 27), 1);
	if (!read_input_args(in_args, argv))
		return (write(2, "Error, Arguments not suitable\n", 30), 1);
	if (!create_philosophers(in_args))
		return (write(2, "Error Making Threads\n", 21), 1);
	return (0);
}
