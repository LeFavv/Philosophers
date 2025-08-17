/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 01:49:13 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/17 02:07:09 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_struct_5(t_args *args, long *tab)
{
	args->nb_philo = tab[0];
	args->number_of_times_each_philosopher_must_eat = tab[1];
	args->time_to_die = tab[2];
	args->time_to_eat = tab[3];
	args->time_to_sleep = tab[4];
	return (1);
}

int main(int argc, char **argv)
{
	t_args	*args;
	t_all	*all;
	long	*tab;
	if (argc == 5)
	{
		gettimeofday(&all->start, NULL);
		tab = malloc(sizeof(long) * 4);
		if (!tab)
			return (1);
		if (!init_struct_5(&args, tab))
			return (free(tab),1);
	}
	else if (argc == 6)
	{
		gettimeofday(&all->start, NULL);
		tab = malloc(sizeof(long) * 5);
		if (!tab)
			return (1);
		if (!init_struct_5(&args, tab))
			return (free(tab),1);
	}
	return (0);
}