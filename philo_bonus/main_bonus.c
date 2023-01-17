/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 00:40:38 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/17 20:45:05 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static int	create_philosophers(int *in_args)
{
	t_shared		philo_args;

	fill_philo_args(&philo_args, in_args);
	if (!prepare_samphores(&philo_args))
		return (write(2, "Semaphore Error\n", 17), 0);
	launch_philosophers(&philo_args);
	close_semaphores(&philo_args);
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
