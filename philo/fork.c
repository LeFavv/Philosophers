/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 02:09:49 by vafavard          #+#    #+#             */
/*   Updated: 2025/09/23 17:01:10 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo);
void	put_forks(t_philo *philo);
void	put_forks_odds(t_philo *philo);

void	take_forks(t_philo *philo)
{
	int	first_fork;
	int	second_fork;

	// Micro-délai uniquement pour les petits groupes
	// if (philo->all->args.nb_philo <= 10)
	// 	usleep(100);
	// Alternance de l'ordre de prise des fourchettes selon l'ID
	if (philo->id % 2 == 0)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	pthread_mutex_lock(&philo->all->forks[first_fork]);
	print_status(&philo, "\e[33mhas taken a fork\033[00m");
	pthread_mutex_lock(&philo->all->forks[second_fork]);
	print_status(&philo, "\e[33mhas taken a fork\033[00m");
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
