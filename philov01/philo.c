/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:11:50 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/30 02:21:25 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

// implementer les rounds pour savoir qui a eat
// checks parsing dans le main avec messages adequats
// totu mettre dans dosseir philo
// gerer le cas de sleep pour quitter si quelqu'un dort mais que le programme est terminado

int		eat(t_philo *philo);
int		no_dead(t_philo **philo);
void	*philosopher_routine_argc_6(void *arg);
void	*philosopher_routine(void *arg);
int		create_threads(t_all *all);
void	*monitor_routine(void *arg);
void	print_status_6(t_philo **philo, char *str);


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

void *philosopher_routine_argc_6(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    
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
    while (1)  // Arrêter quand quelqu'un est mort
    {
		    pthread_mutex_lock(&philo->all->death_mutex);
 			int dead = philo->all->there_is_dead;
    		pthread_mutex_unlock(&philo->all->death_mutex);

    if (dead || philo->all->args.nb_philo <= 1)
        break;
        // print_status(&philo, "is thinking 💻");
		print_status_6(&philo, "is thinking");
		// if (philo->all->args.nb_philo % 2 != 0)
		// 	usleep(1000);
        take_forks(philo);
        // print_status(&philo, "\e[32mis eating\033[00m 🍔");
		print_status_6(&philo, "\e[32mis eating\033[00m");
		
		if (!eat(philo))
			break;
		
        // eat(philo);
		if (philo->all->args.nb_philo % 2 == 0)
			put_forks(philo);
		else
			put_forks_odds(philo);
        // put_forks(philo);
        // print_status(&philo, "\e[34mis sleeping\033[00m 💤");
		print_status_6(&philo, "\e[34mis sleeping\033[00m");
        // usleep(philo->all->args.time_to_sleep * 1000);
		smart_sleep(philo->all->args.time_to_sleep, &philo->all);
    }
    return (NULL);
}
void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    
	if (philo->all->args.number_of_times_each_philosopher_must_eat != -1)
	{	
				// printf("%ld\n", philo->all->args.nb_philo);
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
        break;
        // print_status(&philo, "is thinking 💻");
		print_status(&philo, "is thinking");
		// if (philo->all->args.nb_philo % 2 != 0)
		// 	usleep(1000);
        take_forks(philo);
        // print_status(&philo, "\e[32mis eating\033[00m 🍔");
		print_status(&philo, "\e[32mis eating\033[00m");
        eat(philo);
		if (philo->all->args.nb_philo % 2 == 0)
			put_forks(philo);
		else
			put_forks_odds(philo);
        // put_forks(philo);
        // print_status(&philo, "\e[34mis sleeping\033[00m 💤");
		print_status(&philo, "\e[34mis sleeping\033[00m");
        // usleep(philo->all->args.time_to_sleep * 1000);
		smart_sleep(philo->all->args.time_to_sleep, &philo->all);
    }
    return (NULL);
}

void print_status(t_philo **philo, char *str)
{
    long time;
    pthread_mutex_lock(&(*philo)->all->print_mutex);
    gettimeofday(&(*philo)->all->end, NULL);
    time = time_diff_ms(&(*philo)->all->start, &(*philo)->all->end);

    pthread_mutex_lock(&(*philo)->all->death_mutex);
    int dead = (*philo)->all->there_is_dead;
    pthread_mutex_unlock(&(*philo)->all->death_mutex);

    if (!dead)
        printf("%ld %d %s\n", time, (*philo)->id, str);
    if (dead && (ft_strcmp(str, "died") == 0))
        printf("%s%ld %d %s%s\n", RED, time, (*philo)->id, str, END_COLOR);

	// pthread_mutex_unlock(&(*philo)->all->eating_mutex);
    pthread_mutex_unlock(&(*philo)->all->print_mutex);
}

void print_status_6(t_philo **philo, char *str)
{
    long time;
    pthread_mutex_lock(&(*philo)->all->print_mutex);
    gettimeofday(&(*philo)->all->end, NULL);
    time = time_diff_ms(&(*philo)->all->start, &(*philo)->all->end);

    pthread_mutex_lock(&(*philo)->all->death_mutex);
    int dead = (*philo)->all->there_is_dead;
    pthread_mutex_unlock(&(*philo)->all->death_mutex);

	pthread_mutex_lock(&(*philo)->all->eating_mutex);
	int can_print;
	if (all_ate(*philo) && (*philo)->all->nb_round_eat == 
		(*philo)->all->args.number_of_times_each_philosopher_must_eat)
		return ;
	if ((*philo)->all->nb_round_eat == (*philo)->all->args.number_of_times_each_philosopher_must_eat)
		can_print = 0;
	else
		can_print = 1;
	pthread_mutex_unlock(&(*philo)->all->eating_mutex);
    if (!dead && can_print)
        printf("%ld %d %s %d\n", time, (*philo)->id, str, (*philo)->all->nb_round_eat);
    if (dead && (ft_strcmp(str, "died") == 0) && can_print)
        printf("%s%ld %d %s%s\n", RED, time, (*philo)->id, str, END_COLOR);

	// pthread_mutex_unlock(&(*philo)->all->eating_mutex);
    pthread_mutex_unlock(&(*philo)->all->print_mutex);
}


void *monitor_routine(void *arg)
{
    t_all *all = (t_all *)arg;
    int i;
    t_philo *current_philo;

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

int main(int argc, char **argv)
{
	t_args	*args;
	t_all	*all;
	long	*tab;
	int		i = 0;
	// long	 time;

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
		// free(args);
		free(tab);
		if (!init_philosophers(all))
    		return (free(all), 1);
		// if (all->args.nb_philo == 1)
		// {
		// 	gettimeofday(&all->start, NULL);
		// 	printf("0 1 is thinking 💻\n");
		// 	// printf("%ld\n", all->args.time_to_die);
		// 	// printf("%d\n", all->start.tv_usec);
    	// 	usleep(all->args.time_to_die * 1000);
		// 	gettimeofday(&all->end, NULL);
		// 	time = time_diff_ms(&all->start, &all->end);
		// 	// printf("time = %ld\n", time);
		// 	printf("%s%ld 1 died%s\n", RED, time, END_COLOR);
		// 	free(all);
		// 	free(args);
    	// 	return (0);
		// }
		// printf("nb_philo = %ld\n", all->args.nb_philo);
		// printf("time_to_die = %ld\n", all->args.time_to_die);
		// printf("time_to_eat = %ld\n", all->args.time_to_eat);
		// printf("time_to_sleep = %ld\n", all->args.time_to_sleep);
		// printf("number_of_times_each_philosopher_must_eat = %ld\n", all->args.number_of_times_each_philosopher_must_eat);
		
		pthread_t monitor_thread;
		create_threads(all);
		// create_threads_odds(all);
		// create_threads_pairs(all);
		pthread_create(&monitor_thread, NULL, monitor_routine, all);
		join_threads(all);
		pthread_join(monitor_thread, NULL);
		mutex_destroy(all);
		free(all->philo);
		free(all->forks);
		free(all->ate);
		free(all);
		free(args);
	}
	return (0);
}
