/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 01:49:13 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/17 03:04:15 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int argc, char **argv)
{
	t_args	*args;
	t_all	*all;
	long	*tab;
	int i = 0;

	if (argc == 5)
	{
		gettimeofday(&all->start, NULL);
		tab = malloc(sizeof(long) * 4);
		if (!tab)
			return (1);
		while (i < 4)
		{
			tab[i] = ft_atol(argv[i + 2]);
			i++;
		}
		if (!init_struct_5(&args, tab))
			return (free(tab),1);
	}
	else if (argc == 6)
	{
		gettimeofday(&all->start, NULL);
		tab = malloc(sizeof(long) * 5);
		if (!tab)
			return (1);
		while (i < 5)
		{
			tab[i] = ft_atol(argv[i + 2]);
			i++;
		}
		if (!init_struct_5(&args, tab))
			return (free(tab),1);
	}
	return (0);
}
