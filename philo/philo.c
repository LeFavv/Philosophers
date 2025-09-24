/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:11:50 by vafavard          #+#    #+#             */
/*   Updated: 2025/09/24 15:21:39 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pthread_gestion(t_all *all);
int		alloc_all(t_args **args, t_all **all, long **tab, int size);
int		check_args(long *tab, int i);
int		value_i(int *i);

void	pthread_gestion(t_all *all)
{
	pthread_t	monitor_thread;

	create_threads(all);
	pthread_create(&monitor_thread, NULL, monitor_routine, all);
	join_threads(all);
	pthread_join(monitor_thread, NULL);
}

int	alloc_all(t_args **args, t_all **all, long **tab, int size)
{
	*args = malloc(sizeof(t_args));
	if (!*args)
		return (0);
	*all = malloc(sizeof(t_all));
	if (!*all)
		return (0);
	memset(*all, 0, sizeof(t_all));
	gettimeofday(&(*all)->start, NULL);
	*tab = malloc(sizeof(long) * (size));
	if (!*tab)
		return (0);
	return (1);
}

int	value_i(int *i)
{
	*i = 0;
	return (0);
}

int	check_args(long *tab, int i)
{
	if (tab[i] < 0)
	{
		printf("%sNo negatives\n%s", RED, END_COLOR);
		return (0);
	}
	if (tab[i] == 0)
	{
		printf("%sError value\n%s", RED, END_COLOR);
		return (0);
	}
	if (tab[0] > 200)
	{
		printf("%sToo many philos : Max => 200\n%s", RED, END_COLOR);
		return (0);
	}
	if (i != 0 && tab[i] > 2000)
	{
		printf("%svalue should be > 0 && <= 2000\n%s", RED, END_COLOR);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_args	*args;
	t_all	*all;
	long	*tab;
	int		i;

	if (argc == 5 || argc == 6)
	{
		if (!alloc_all(&args, &all, &tab, argc - 1) || value_i(&i))
			return (ft_free_all(all, args, tab), 1);
		while (i < argc - 1)
		{
			tab[i] = ft_atol(argv[i + 1]);
			if (!check_args(tab, i))
				return ((ft_free_all(all, args, tab)), 1);
			i++;
		}
		if (!init_struct_5(args, tab, argc - 1))
			return (ft_free_all(all, args, tab), 1);
		all->args = *args;
		if (!init_philosophers(all))
			return (ft_free_all(all, args, tab), 1);
		pthread_gestion(all);
		return (ft_free_all(all, args, tab), 0);
	}
	return (printf("%sBad arguments\n%s", RED, END_COLOR), 1);
}
