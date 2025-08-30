/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:11:50 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/30 13:31:49 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int argc, char **argv)
{
	if (argc == 5 || argc == 6)
	{
		pthread_t monitor_thread;
		t_args	*args;
		t_all	*all;
		long	*tab;
		int		i;
		
		i = 0;
		args = malloc(sizeof(t_args));
		if (!args)
			return (1);
		all = malloc(sizeof(t_all));
		if (!all)
			return (free(args), 1);
		gettimeofday(&all->start, NULL);
		tab = malloc(sizeof(long) * (argc - 1));
		if (!tab)
			return (free(args), free(all), 1);
		while (i < argc - 1)
		{
			tab[i] = ft_atol(argv[i + 1]);
			if (tab[i] < 0)
				return (printf("%sNo negatives\n%s", RED, END_COLOR), 1);
			i++;
		}
		if (!init_struct_5(args, tab, argc - 1))
			return (free(tab), free(args), free(all), 1);
		all->args = *args;
		if (!init_philosophers(all))
    		return (ft_free_all(all, args, tab), 1);
		create_threads(all);
		pthread_create(&monitor_thread, NULL, monitor_routine, all);
		join_threads(all);
		pthread_join(monitor_thread, NULL);
		mutex_destroy(all);
		ft_free_all(all, args, tab);
	}
	else
		printf("%sBad arguments\n%s", RED, END_COLOR);
	return (0);
}

// int main(int argc, char **argv)
// {
// 	t_args	*args;
// 	t_all	*all;
// 	long	*tab;
// 	int		i = 0;

// 	if (argc == 5 || argc == 6)
// 	{
// 		// Alloc structures
// 		args = malloc(sizeof(t_args));
// 		if (!args)
// 			return (1);
// 		all = malloc(sizeof(t_all));
// 		if (!all)
// 			return (free(args), 1);
		
// 		gettimeofday(&all->start, NULL);
// 		tab = malloc(sizeof(long) * (argc - 1));
// 		if (!tab)
// 			return (free(args), free(all), 1);
// 		while (i < argc - 1)
// 		{
			
// 			tab[i] = ft_atol(argv[i + 1]);
// 			if (tab[i] < 0)
// 				return (printf("%sNo negatives\n%s", RED, END_COLOR), 1);
// 			i++;
// 		}
// 		if (!init_struct_5(args, tab, argc - 1))
// 			return (free(tab), free(args), free(all), 1);
// 		all->args = *args;
// 		// free(args);
// 		free(tab);
// 		if (!init_philosophers(all))
//     		return (free(all), 1);
// 		// if (all->args.nb_philo == 1)
// 		// {
// 		// 	gettimeofday(&all->start, NULL);
// 		// 	printf("0 1 is thinking 💻\n");
// 		// 	// printf("%ld\n", all->args.time_to_die);
// 		// 	// printf("%d\n", all->start.tv_usec);
//     	// 	usleep(all->args.time_to_die * 1000);
// 		// 	gettimeofday(&all->end, NULL);
// 		// 	time = time_diff_ms(&all->start, &all->end);
// 		// 	// printf("time = %ld\n", time);
// 		// 	printf("%s%ld 1 died%s\n", RED, time, END_COLOR);
// 		// 	free(all);
// 		// 	free(args);
//     	// 	return (0);
// 		// }
// 		// printf("nb_philo = %ld\n", all->args.nb_philo);
// 		// printf("time_to_die = %ld\n", all->args.time_to_die);
// 		// printf("time_to_eat = %ld\n", all->args.time_to_eat);
// 		// printf("time_to_sleep = %ld\n", all->args.time_to_sleep);
// 		// printf("number_of_times_each_philosopher_must_eat = %ld\n", all->args.number_of_times_each_philosopher_must_eat);
		
// 		pthread_t monitor_thread;
// 		create_threads(all);
// 		// create_threads_odds(all);
// 		// create_threads_pairs(all);
// 		pthread_create(&monitor_thread, NULL, monitor_routine, all);
// 		join_threads(all);
// 		pthread_join(monitor_thread, NULL);
// 		mutex_destroy(all);
// 		ft_free_all(all, args);
// 		// free(all->philo);
// 		// free(all->forks);
// 		// free(all->ate);
// 		// free(all);
// 		// free(args);
// 	}
// 	else
// 		printf("%sBad arguments\n%s", RED, END_COLOR);
// 	return (0);
// }
