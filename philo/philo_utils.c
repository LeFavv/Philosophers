/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:41:27 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/31 15:48:10 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atol(char *str);
int		ft_strcmp(char *s1, char *s2);
void	last_meal(t_philo *philo);
void	if_dead(t_philo *philo, int *dead);

long	ft_atol(char *str)
{
	long	nb;
	long	sign;
	int		i;

	nb = 0;
	sign = 1;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return (nb * sign);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	gettimeofday(&philo->last_meal, NULL);
	pthread_mutex_unlock(&philo->meal_mutex);
}

void	if_dead(t_philo *philo, int *dead)
{
	pthread_mutex_lock(&philo->all->death_mutex);
	*dead = philo->all->there_is_dead;
	pthread_mutex_unlock(&philo->all->death_mutex);
}
