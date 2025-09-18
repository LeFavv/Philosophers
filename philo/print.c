/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 03:18:59 by vafavard          #+#    #+#             */
/*   Updated: 2025/09/03 13:55:46 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo **philo, char *str);
void	print_status_6(t_philo **philo, char *str);

void	print_status(t_philo **philo, char *str)
{
	long	time;
	int		dead;

	pthread_mutex_lock(&(*philo)->all->print_mutex);
	pthread_mutex_lock(&(*philo)->all->death_mutex);
	dead = (*philo)->all->there_is_dead;
	pthread_mutex_unlock(&(*philo)->all->death_mutex);
	gettimeofday(&(*philo)->all->end, NULL);
	time = time_diff_ms(&(*philo)->all->start, &(*philo)->all->end);
	if (!dead)
		printf("%ld %d %s\n", time, (*philo)->id, str);
	if (dead && (ft_strcmp(str, "died") == 0))
		printf("%s%ld %d %s%s\n", RED, time, (*philo)->id, str, END_COLOR);
	pthread_mutex_unlock(&(*philo)->all->print_mutex);
}

void	print_status_6(t_philo **philo, char *str)
{
	long	time;
	int		dead;
	int		can_print;

	pthread_mutex_lock(&(*philo)->all->print_mutex);
	gettimeofday(&(*philo)->all->end, NULL);
	time = time_diff_ms(&(*philo)->all->start, &(*philo)->all->end);
	pthread_mutex_lock(&(*philo)->all->death_mutex);
	dead = (*philo)->all->there_is_dead;
	pthread_mutex_unlock(&(*philo)->all->death_mutex);
	pthread_mutex_lock(&(*philo)->all->eating_mutex);
	if (all_ate(*philo) && (*philo)->all->nb_round_eat
		== (*philo)->all->args.nb_time_eat)
		return ;
	if ((*philo)->all->nb_round_eat == (*philo)->all->args.nb_time_eat)
		can_print = 0;
	else
		can_print = 1;
	pthread_mutex_unlock(&(*philo)->all->eating_mutex);
	if (!dead && can_print)
		printf("%ld %d %s %d\n", time, (*philo)->id, str,
			(*philo)->all->nb_round_eat);
	if (dead && (ft_strcmp(str, "died") == 0) && can_print)
		printf("%s%ld %d %s%s\n", RED, time, (*philo)->id, str, END_COLOR);
	pthread_mutex_unlock(&(*philo)->all->print_mutex);
}
