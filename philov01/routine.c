/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 03:15:31 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/30 13:50:58 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine_argc_6(void *arg);
void	*philosopher_routine(void *arg);
void    *monitor_routine(void *arg);

void *philosopher_routine_argc_6(void *arg)
{
    t_philo *philo;
    
	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->meal_mutex);
	gettimeofday(&philo->last_meal, NULL);
	pthread_mutex_unlock(&philo->meal_mutex);
	if (philo->all->args.nb_philo == 1)
	{
		print_status_6(&philo, "has taken a fork");
		usleep(philo->all->args.time_to_die * 1000);
		print_status_6(&philo, "died");
		philo->all->there_is_dead = 1;
		return (NULL);
	}
    while (1)
    {
		pthread_mutex_lock(&philo->all->death_mutex);
 		int dead = philo->all->there_is_dead;
    	pthread_mutex_unlock(&philo->all->death_mutex);
    	if (dead || philo->all->args.nb_philo <= 1)
    	    break;
    	print_status(&philo, "is thinking 💻");
    	take_forks(philo);
    	print_status(&philo, "\e[32mis eating\033[00m 🍔");
		if (!eat(philo))
		{
		    if (philo->all->args.nb_philo % 2 == 0)
		        put_forks(philo);
		    else
		        put_forks_odds(philo);
		    break;  // on quitte seulement après avoir libéré
		}
		if (philo->all->args.nb_philo % 2 == 0)
			put_forks(philo);
		else
			put_forks_odds(philo);
        print_status(&philo, "\e[34mis sleeping\033[00m 💤");
		smart_sleep(philo->all->args.time_to_sleep, &philo->all);
    }
    return (NULL);
}
void *philosopher_routine(void *arg)
{
    t_philo *philo;
    
	philo = (t_philo *)arg;
	if (philo->all->args.number_of_times_each_philosopher_must_eat != -1)
	{	
		philosopher_routine_argc_6(arg);
		return (NULL);
	}
	pthread_mutex_lock(&philo->meal_mutex);
	gettimeofday(&philo->last_meal, NULL);
	pthread_mutex_unlock(&philo->meal_mutex);
	if (philo->all->args.nb_philo == 1)
	{
		print_status(&philo, "has taken a fork");
		usleep(philo->all->args.time_to_die * 1000);
		print_status(&philo, "died");
		philo->all->there_is_dead = 1;
		return (NULL);
	}
    while (1)  // Arrêter quand quelqu'un est mort
    {
		    pthread_mutex_lock(&philo->all->death_mutex);
 			int dead = philo->all->there_is_dead;
    		pthread_mutex_unlock(&philo->all->death_mutex);

    	if (dead || philo->all->args.nb_philo <= 1)
    		break ;
    	print_status(&philo, "is thinking 💻");
        take_forks(philo);
        print_status(&philo, "\e[32mis eating\033[00m 🍔");
       if (!eat(philo))
			break;
		if (philo->all->args.nb_philo % 2 == 0)
			put_forks(philo);
		else
			put_forks_odds(philo);
        print_status(&philo, "\e[34mis sleeping\033[00m 💤");
		smart_sleep(philo->all->args.time_to_sleep, &philo->all);
    }
    return (NULL);
}

void *monitor_routine(void *arg)
{
    t_all *all;
    int i;
    t_philo *current_philo;

	all = (t_all *)arg;
    while (1)
    {
        pthread_mutex_lock(&all->death_mutex);
        if (all->there_is_dead)
        {
            pthread_mutex_unlock(&all->death_mutex);
            break;
        }
        pthread_mutex_unlock(&all->death_mutex);
        i = 0;
        while (i < all->args.nb_philo)
        {
            current_philo = &all->philo[i];
            if (!no_dead(&current_philo))
            {
                print_status(&current_philo, "died");
                break;
            }
            i++;
        }
        usleep(200);
    }
    return (NULL);
}
