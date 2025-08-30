/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 03:13:15 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/30 03:14:49 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	all_ate(t_philo *philo);
int	eat(t_philo *philo);
int	no_dead(t_philo **philo);

int	all_ate(t_philo *philo)
{
	int i = 0;

	while (i < philo->all->args.nb_philo)
	{
		if (philo->all->ate[i] == NEED_TO_EAT)
			return (0);
		i++;
	}
	philo->all->nb_round_eat += 1;
	if (philo->all->nb_round_eat == philo->all->args.number_of_times_each_philosopher_must_eat)
		exit (0);
	printf("J'arrive la\n");
	init_ate(&philo->all);
	return (1);
}

int	eat(t_philo *philo)
{
    pthread_mutex_lock(&philo->meal_mutex);
    philo->meals_eaten += 1;
    gettimeofday(&philo->last_meal, NULL);
	if (philo->all->args.number_of_times_each_philosopher_must_eat != -1)
		philo->all->ate[philo->id - 1] = ATE;
    pthread_mutex_unlock(&philo->meal_mutex);
    smart_sleep(philo->all->args.time_to_eat, &philo->all);
	
	pthread_mutex_lock(&philo->all->eating_mutex);
	if (philo->all->args.number_of_times_each_philosopher_must_eat != -1)
	{
		if (all_ate(philo) && philo->all->nb_round_eat == philo->all->args.number_of_times_each_philosopher_must_eat)
			return (0);
	}
	pthread_mutex_unlock(&philo->all->eating_mutex);

    if (philo->all->args.number_of_times_each_philosopher_must_eat != -1
        && philo->all->nb_round_eat >= philo->all->args.number_of_times_each_philosopher_must_eat)
        return (0); // Ce philo a fini de manger
    return (1);
}

int	no_dead(t_philo **philo)
{
    struct timeval current_time;
	long time_since_last_meal;

    gettimeofday(&current_time, NULL);
    pthread_mutex_lock(&(*philo)->meal_mutex);
    time_since_last_meal = time_diff_ms(&(*philo)->last_meal, &current_time);
    pthread_mutex_unlock(&(*philo)->meal_mutex);
    // printf("Philo %d: %ld ms since last meal", philo->id, time_since_last_meal);
    
    if (time_since_last_meal > (*philo)->all->args.time_to_die)
	{
		pthread_mutex_lock(&(*philo)->all->death_mutex);
		(*philo)->all->there_is_dead += 1;
		pthread_mutex_unlock(&(*philo)->all->death_mutex);
        return (0);  // Mort
	}
    return (1);
}
