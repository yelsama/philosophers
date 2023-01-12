/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_atoi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 17:33:51 by ymohamed          #+#    #+#             */
/*   Updated: 2023/01/09 23:55:00 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	calcu(ssize_t *nb, ssize_t *md, int *i, char *ptr);

ssize_t	philo_atoi(const char *str, int *i)
{
	char		*ptr;
	ssize_t		sn;
	ssize_t		nb;
	ssize_t		md;

	ptr = (char *)str;
	sn = 1;
	nb = 0;
	md = 0;
	while (ptr[*i] == ' ' || (ptr[*i] > 8 && ptr[*i] < 14))
		(*i)++;
	if (ptr[*i] == '-' || ptr[*i] == '+')
		if (ptr[(*i)++] == '-')
			sn = sn * -1;
	if (ptr[*i] < '0' || ptr[*i] > '9')
		return (-1);
	while (ptr[*i] == '0')
		(*i)++;
	calcu(&nb, &md, i, ptr);
	if (md > 10 || nb > INT_MAX)
		return (-1);
	return (nb * sn);
}

static void	calcu(ssize_t *nb, ssize_t *md, int *i, char *ptr)
{
	while (ptr[*i] >= '0' && ptr[*i] <= '9' && *md < 11)
	{
		*nb = *nb * 10 + (ptr[*i] - 48);
		(*md)++;
		(*i)++;
	}
	if (!(ptr[*i] == '\0' || ptr[*i] == ' '))
		*md = 11;
}
