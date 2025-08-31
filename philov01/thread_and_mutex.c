/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_and_mutex.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 02:16:15 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/31 15:51:02 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	mutex_destroy(t_all *all);
int	join_threads(t_all *all);
int	create_threads(t_all *all);

int	mutex_destroy(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->args.nb_philo)
	{
		if (pthread_mutex_destroy(&all->forks[i]) != 0)
			return (0);
		if (pthread_mutex_destroy(&all->philo[i].meal_mutex) != 0)
			return (0);
		i++;
	}
	if (pthread_mutex_destroy(&all->death_mutex) != 0)
		return (0);
	if (pthread_mutex_destroy(&all->print_mutex) != 0)
		return (0);
	if (pthread_mutex_destroy(&all->eating_mutex) != 0)
		return (0);
	return (1);
}

int	join_threads(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->args.nb_philo)
	{
		if (pthread_join(all->philo[i].thread, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	create_threads(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->args.nb_philo)
	{
		if (pthread_create(&all->philo[i].thread, NULL,
				philosopher_routine, &all->philo[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}
