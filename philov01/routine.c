/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 03:15:31 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/31 11:07:31 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine_argc_6(void *arg);
void	*philosopher_routine(void *arg);
void	*monitor_routine(void *arg);
void	last_meal(t_philo *philo);
void	put_the_right_fork(t_philo *philo);

void	last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	gettimeofday(&philo->last_meal, NULL);
	pthread_mutex_unlock(&philo->meal_mutex);
}

void	put_the_right_fork(t_philo *philo)
{
	if (philo->all->args.nb_philo % 2 == 0)
		put_forks(philo);
	else
		put_forks_odds(philo);
}

void	philosopher_routine_solo(t_philo *philo)
{
	print_status(&philo, "has taken a fork");
	usleep(philo->all->args.time_to_die * 1000);
	print_status(&philo, "died");
	philo->all->there_is_dead = 1;
}

void	*philosopher_routine_argc_6(void *arg)
{
	t_philo	*philo;
	int		dead;

	philo = (t_philo *)arg;
	last_meal(philo);
	if (philo->all->args.nb_philo == 1)
		return (philosopher_routine_solo(philo), NULL);
	while (1)
	{
		pthread_mutex_lock(&philo->all->death_mutex);
		dead = philo->all->there_is_dead;
		pthread_mutex_unlock(&philo->all->death_mutex);
		if (dead || philo->all->args.nb_philo <= 1)
			break ;
		print_status(&philo, "is thinking 💻");
		take_forks(philo);
		print_status(&philo, "\e[32mis eating\033[00m 🍔");
		if (!eat(philo))
			return (put_the_right_fork(philo), NULL);
		put_the_right_fork(philo);
		print_status(&philo, "\e[34mis sleeping\033[00m 💤");
		smart_sleep(philo->all->args.time_to_sleep, &philo->all);
	}
	return (NULL);
}

void	if_dead(t_philo *philo, int *dead)
{
	pthread_mutex_lock(&philo->all->death_mutex);
	*dead = philo->all->there_is_dead;
	pthread_mutex_unlock(&philo->all->death_mutex);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;
	int		dead;

	philo = (t_philo *)arg;
	if (philo->all->args.nb_time_eat != -1)
		return (philosopher_routine_argc_6(arg), NULL);
	last_meal(philo);
	if (philo->all->args.nb_philo == 1)
		return (philosopher_routine_solo(philo), NULL);
	while (1)
	{
		if_dead(philo, &dead);
		if (dead || philo->all->args.nb_philo <= 1)
			break ;
		print_status(&philo, "is thinking 💻");
		take_forks(philo);
		print_status(&philo, "\e[32mis eating\033[00m 🍔");
		if (!eat(philo))
			break ;
		put_the_right_fork(philo);
		print_status(&philo, "\e[34mis sleeping\033[00m 💤");
		smart_sleep(philo->all->args.time_to_sleep, &philo->all);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_all	*all;
	int		i;
	t_philo	*current_philo;

	all = (t_all *)arg;
	while (1)
	{
		pthread_mutex_lock(&all->death_mutex);
		if (all->there_is_dead)
			return (pthread_mutex_unlock(&all->death_mutex), NULL);
		pthread_mutex_unlock(&all->death_mutex);
		i = 0;
		while (i < all->args.nb_philo)
		{
			current_philo = &all->philo[i];
			if (!no_dead(&current_philo))
			{
				print_status(&current_philo, "died");
				break ;
			}
			i++;
		}
		usleep(200);
	}
	return (NULL);
}
