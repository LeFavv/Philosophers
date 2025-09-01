/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 02:09:49 by vafavard          #+#    #+#             */
/*   Updated: 2025/09/01 12:52:08 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo);
void	put_forks(t_philo *philo);
void	put_forks_odds(t_philo *philo);

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		usleep(500);
		pthread_mutex_lock(&philo->all->forks[philo->left_fork]);
		print_status(&philo, "\e[33mhas taken a fork\033[00m 🍴");
		pthread_mutex_lock(&philo->all->forks[philo->right_fork]);
		print_status(&philo, "\e[33mhas taken a fork\033[00m🍴");
	}
	else if (philo->id % 2 != 0 && philo->id == philo->all->args.nb_philo)
	{
		usleep(500);
		pthread_mutex_lock(&philo->all->forks[philo->left_fork]);
		print_status(&philo, "\e[33mhas taken a fork\033[00m 🍴");
		pthread_mutex_lock(&philo->all->forks[philo->right_fork]);
		print_status(&philo, "\e[33mhas taken a fork\033[00m🍴");
	}
	else
	{
		pthread_mutex_lock(&philo->all->forks[philo->right_fork]);
		print_status(&philo, "\e[33mhas taken a fork\033[00m🍴");
		pthread_mutex_lock(&philo->all->forks[philo->left_fork]);
		print_status(&philo, "\e[33mhas taken a fork\033[00m 🍴");
	}
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->all->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->all->forks[philo->right_fork]);
}

void	put_forks_odds(t_philo *philo)
{
	pthread_mutex_unlock(&philo->all->forks[philo->right_fork]);
	pthread_mutex_unlock(&philo->all->forks[philo->left_fork]);
}
