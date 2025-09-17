/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 03:03:55 by vafavard          #+#    #+#             */
/*   Updated: 2025/09/17 11:21:57 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		init_struct_5(t_args *args, long *tab, int nb);
int		init_philosophers(t_all *all);
void	init_ate(t_all **all);
int		mutex_init(t_all *all);
int		mutex_init_2(t_all *all);

int	init_struct_5(t_args *args, long *tab, int nb)
{
	args->nb_philo = tab[0];
	args->time_to_die = tab[1];
	args->time_to_eat = tab[2];
	args->time_to_sleep = tab[3];
	if (nb == 5)
		args->nb_time_eat = tab[4];
	else
		args->nb_time_eat = -1;
	return (1);
}

int	mutex_init(t_all *all)
{
	if (pthread_mutex_init(&all->death_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&all->print_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&all->eating_mutex, NULL))
		return (0);
	return (1);
}

int	mutex_init_2(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->args.nb_philo)
	{
		if (pthread_mutex_init(&all->forks[i], NULL) != 0)
			return (0);
		if (pthread_mutex_init(&all->philo[i].meal_mutex, NULL) != 0)
			return (0);
		all->philo[i].id = i + 1;
		all->philo[i].left_fork = i;
		all->philo[i].right_fork = (i + 1) % all->args.nb_philo;
		all->philo[i].all = all;
		all->philo[i].meals_eaten = 0;
		gettimeofday(&all->philo[i].last_meal, NULL);
		i++;
	}
	return (1);
}

int	init_philosophers(t_all *all)
{
	all->philo = malloc(sizeof(t_philo) * all->args.nb_philo);
	if (!all->philo)
		return (0);
	all->forks = malloc(sizeof(pthread_mutex_t) * all->args.nb_philo);
	if (!all->forks)
		return (free(all->philo), 0);
	if (!mutex_init(all))
		return (0);
	if (!mutex_init_2(all))
		return (0);
	if (all->args.nb_time_eat != -1)
	{
		all->ate = malloc(sizeof(int) * all->args.nb_philo);
		if (!all->ate)
			return (0);
		init_ate(&all);
	}
	all->there_is_dead = 0;
	all->nb_round_eat = 0;
	return (1);
}

void	init_ate(t_all **all)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&(*all)->philo->meal_mutex);
	// pthread_mutex_lock(&(*all)->eating_mutex);
	while (i < (*all)->args.nb_philo)
	{
		(*all)->ate[i] = NEED_TO_EAT;
		i++;
	}
	// pthread_mutex_unlock(&(*all)->eating_mutex);
	pthread_mutex_unlock(&(*all)->philo->meal_mutex);
}
