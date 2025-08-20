/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 01:49:13 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/20 11:43:54 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	time_diff_ms(struct timeval *start, struct timeval *end);
void	sleep_routine(t_all **all);
void	*eat_routine(t_all **all);
int		mutex_destroy(t_all *all);
int		init_philosophers(t_all *all);
int		init_philosophers(t_all *all);
void	*philosopher_routine(void *arg);
void	print_status(t_philo **philo, char *str);
void	take_forks(t_philo *philo);
int		join_threads(t_all *all);
int		create_threads(t_all *all);
void	put_forks(t_philo *philo);
void	can_eat_same_time(t_all **all);

long time_diff_ms(struct timeval *start, struct timeval *end) 
{
    long seconds = end->tv_sec - start->tv_sec;
    long microseconds = end->tv_usec - start->tv_usec;
	
    return (seconds * 1000) + ((microseconds + 500) / 1000);
}

int	mutex_destroy(t_all *all)
{
	int i = 0;
	
	while (i < all->args.nb_philo)
	{
		if (pthread_mutex_destroy(&all->forks[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	init_philosophers(t_all *all)
{
	int	i;

	i = 0;
	all->philo = malloc(sizeof(t_philo) * all->args.nb_philo);
	if (!all->philo)
		return (0);
	all->forks = malloc(sizeof(pthread_mutex_t) * all->args.nb_philo);
	if (!all->forks)
		return (0);
	while (i < all->args.nb_philo)
	{
		if (pthread_mutex_init(&all->forks[i], NULL) != 0)
			return (0);
		i++;
	}
	i = 0;
	all->there_is_dead = 0;
	 while (i < all->args.nb_philo)
    {
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

void	eat(t_philo *philo)
{
    philo->meals_eaten += 1;
    usleep(philo->all->args.time_to_eat * 1000);
	gettimeofday(&philo->last_meal, NULL);
	// printf("Philo %d finished eating at %ld\n", philo->id, philo->last_meal.tv_usec);
}

int	no_dead(t_philo **philo)
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    
    long time_since_last_meal = time_diff_ms(&(*philo)->last_meal, &current_time);
    
    // printf("Philo %d: %ld ms since last meal", philo->id, time_since_last_meal);
    
    if (time_since_last_meal >= (*philo)->all->args.time_to_die)
	{
		(*philo)->all->there_is_dead += 1;
        return (0);  // Mort
	}
    return (1);
}

void *philo_routine_argc_6(void *arg)
{
    t_philo *philo = (t_philo *)arg;
	int nb_philo;
	int round;
	
	nb_philo = philo->all->args.nb_philo;
	round = 0;
	
    while (round != nb_philo && !no_dead(&philo))
    {
        // Penser
        print_status(&philo, "is thinking");
        
        // Manger
        take_forks(philo);
        print_status(&philo, "is eating");
        eat(philo);
        put_forks(philo);
        
        // Dormir
        print_status(&philo, "is sleeping");
        usleep(philo->all->args.time_to_sleep * 1000);
    }
    return (NULL);
}


void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    
	if (philo->all->args.number_of_times_each_philosopher_must_eat != -1)
	{	
		philo_routine_argc_6(arg);
		return (NULL);
	}
	// if (philo->all->args.nb_philo == 1)
	// {
	// 	print_status(&philo, "is thinking");
	// 	usleep(philo->all->args.time_to_sleep * 1000);
	// 	if (!no_dead(&philo))
    //     {
    //         print_status(&philo, "died");
    //        	return (NULL);
    //     }
		
	// }
    while (1)  // Boucle infinie, on sort quand mort
    {
        print_status(&philo, "is thinking 💻");
        take_forks(philo);
        print_status(&philo, "\e[32mis eating\033[00m 🍔");
        eat(philo);
        put_forks(philo);
        print_status(&philo, "\e[34mis sleeping\033[00m 💤");
        usleep(philo->all->args.time_to_sleep * 1000);
        
        // Vérifier la mort APRÈS avoir dormi (quand on a vraiment faim)
        if (!no_dead(&philo))
        {
            print_status(&philo, "died");
            break ;
        }
    }
    return (NULL);
}

int	ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	print_status(t_philo **philo, char *str)
{
	long time;
	
	gettimeofday(&(*philo)->all->end, NULL);
	time = time_diff_ms(&(*philo)->all->start, &(*philo)->all->end);
	if ((*philo)->all->there_is_dead == 0)
	{
		// printf("valeur de there_is_dead = %d\n", (*philo)->all->there_is_dead);
		printf("%ld %d %s\n", time, (*philo)->id, str);
	}
	if (((*philo)->all->there_is_dead == 1) && (ft_strcmp(str, "died") == 0))
	{
		// printf("valeur de there_is_dead = %d\n", (*philo)->all->there_is_dead);
		printf("%s%ld %d %s%s\n",RED, time, (*philo)->id, str, END_COLOR);
	}
}

void take_forks(t_philo *philo)
{
	// if (philo->all->args.nb_philo == 1)
	// 	return ;
    if (philo->left_fork < philo->right_fork)
    {
        pthread_mutex_lock(&philo->all->forks[philo->left_fork]);
		print_status(&philo, "\e[33mhas taken a fork\033[00m");
        pthread_mutex_lock(&philo->all->forks[philo->right_fork]);
        print_status(&philo, "\e[33mhas taken a fork\033[00m");
    }
    else
    {
        pthread_mutex_lock(&philo->all->forks[philo->right_fork]);
        print_status(&philo, "\e[33mhas taken a fork\033[00m");
        pthread_mutex_lock(&philo->all->forks[philo->left_fork]);
        print_status(&philo, "\e[33mhas taken a fork\033[00m");
    }
}

int	join_threads(t_all *all)
{
	int i = 0;
    
    while (i < all->args.nb_philo)
    {
        if (pthread_join(all->philo[i].thread, NULL) != 0)
            return (0);
        i++;
    }
    return (1);
}

int create_threads(t_all *all)
{
    int i = 0;
    
    while (i < all->args.nb_philo)
    {
        if (pthread_create(&all->philo[i].thread, NULL, philosopher_routine, &all->philo[i]) != 0)
            return (0);
        i++;
    }
    return (1);
}

void put_forks(t_philo *philo)
{
    pthread_mutex_unlock(&philo->all->forks[philo->left_fork]);
    pthread_mutex_unlock(&philo->all->forks[philo->right_fork]);
}

void	can_eat_same_time(t_all **all)
{
	if ((*all)->args.nb_philo % 2 == 0)
		(*all)->eat_same_time = (*all)->args.nb_philo / 2;
	else
		(*all)->eat_same_time = ((*all)->args.nb_philo - 1)/ 2;
}



int main(int argc, char **argv)
{
	t_args	*args;
	t_all	*all;
	long	*tab;
	int		i = 0;

	if (argc == 5 || argc == 6)
	{
		// Alloc structures
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
			i++;
		}
		if (!init_struct_5(args, tab, argc - 1))
			return (free(tab), free(args), free(all), 1);
		all->args = *args;
		free(args);
		free(tab);
		if (!init_philosophers(all))
    		return (free(all), 1);
		// printf("nb_philo = %ld\n", all->args.nb_philo);
		// printf("time_to_die = %ld\n", all->args.time_to_die);
		// printf("time_to_eat = %ld\n", all->args.time_to_eat);
		// printf("time_to_sleep = %ld\n", all->args.time_to_sleep);
		// printf("number_of_times_each_philosopher_must_eat = %ld\n", all->args.number_of_times_each_philosopher_must_eat);
		create_threads(all);
		join_threads(all);
		mutex_destroy(all);
		free(all->philo);
		free(all->forks);
		free(all);
	}
	return (0);
}
