/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:11:50 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/29 19:36:28 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

// implementer les rounds pour savoir qui a eat
// checks parsing dans le main avec messages adequats
// totu mettre dans dosseir philo
// gerer le cas de sleep pour quitter si quelqu'un dort mais que le programme est terminado

long	time_diff_ms(struct timeval *start, struct timeval *end);
int		mutex_destroy(t_all *all);
long	get_time_ms(void);
void	smart_sleep(long time_in_ms, t_all **all);
int		init_philosophers(t_all *all);
int		eat(t_philo *philo);
int		no_dead(t_philo **philo);
void	*philosopher_routine_argc_6(void *arg);
void	*philosopher_routine(void *arg);
int		ft_strcmp(char *s1, char *s2);
void	print_status(t_philo **philo, char *str);
void	take_forks(t_philo *philo);
int		join_threads(t_all *all);
int		create_threads(t_all *all);
void	put_forks(t_philo *philo);
void	put_forks_odds(t_philo *philo);
void	*monitor_routine(void *arg);
void	init_ate(t_all **all);
int		all_ate(t_philo *philo);

void	init_ate(t_all **all) // je dois le malloc a la bonne taille aussi
{
	int i = 0;

	while (i < (*all)->args.nb_philo)
	{
		(*all)->ate[i] = NEED_TO_EAT;
		i++;
	}
}

long time_diff_ms(struct timeval *start, struct timeval *end)
{
    long seconds = end->tv_sec - start->tv_sec;
    long microseconds = end->tv_usec - start->tv_usec;
    return (seconds * 1000) + (microseconds / 1000);
}

int	mutex_destroy(t_all *all)
{
	int i = 0;

	while (i < all->args.nb_philo)
	{
		if (pthread_mutex_destroy(&all->forks[i]) != 0)
			return (0);
		if (pthread_mutex_destroy(&all->philo[i].meal_mutex) != 0)
			return (0);
		i++;
	}
	if (pthread_mutex_destroy(&all->death_mutex) != 0)
		return (0);
	if (pthread_mutex_destroy(&all->print_mutex) != 0)
		return (0);
	if (pthread_mutex_destroy(&all->eating_mutex) != 0)
		return (0);
	return (1);
}
  long	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void smart_sleep(long time_in_ms, t_all **all)
{
    long start_time = get_time_ms();

    while (get_time_ms() - start_time < time_in_ms)
    {
        pthread_mutex_lock(&(*all)->death_mutex);
        int dead = (*all)->there_is_dead;
        pthread_mutex_unlock(&(*all)->death_mutex);

        if (dead)
            break;
        usleep(200);
    }
}

int	init_philosophers(t_all *all)
{
	int	i;

	all->philo = malloc(sizeof(t_philo) * all->args.nb_philo);
	if (!all->philo)
		return (0);

	all->forks = malloc(sizeof(pthread_mutex_t) * all->args.nb_philo);
	if (!all->forks)
		return (free(all->philo), 0);

	// Mutex globaux
	if (pthread_mutex_init(&all->death_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&all->print_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&all->eating_mutex, NULL))
		return (0);

	// Mutex par philosophe et initialisation
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
	//new part de la
	if (all->args.number_of_times_each_philosopher_must_eat != -1)
	{
		all->ate = malloc(sizeof(int) * all->args.number_of_times_each_philosopher_must_eat);
		if (!all->ate)
			return (0);
		init_ate(&all);
		// i = 0;
		// while (i < all->args.nb_philo)
		// {
		// 	all->ate[i] = malloc(sizeof(int) * (int)all->args.nb_philo);
		// 	if (!all->ate[i])
		// 		return (0);
		// 	i++;
		// }
	}
	//a la
	all->there_is_dead = 0;
	all->nb_round_eat = 0;
	return (1);
}

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

// int	eat(t_philo *philo)
// {
//     // philo->meals_eaten += 1;
// 	pthread_mutex_lock(&philo->meal_mutex);
// 	philo->meals_eaten += 1;
// 	philo->all->ate[philo->id] = ATE;
// 	if (all_ate(philo))
// 		philo->all->nb_round_eat += 1;
// 	if (philo->all->nb_round_eat == philo->all->args.number_of_times_each_philosopher_must_eat)
// 		return (0);
// 	// if (all_ate(philo))
// 	// {
// 	// 	init_ate(&philo->all);
// 	// 	philo->all->nb_round_eat += 1;
// 	// 	//thread pour regarder en temps reel si condition remplie
// 	// }
// 	gettimeofday(&philo->last_meal, NULL); // Mettre à jour AVANT de manger
// 	pthread_mutex_unlock(&philo->meal_mutex);
// 	// usleep(philo->all->args.time_to_eat * 1000);
// 	smart_sleep(philo->all->args.time_to_eat, &philo->all);
// 	// printf("Philo %d finished eating at %ld\n", philo->id, philo->last_meal.tv_usec);
// 	return (1);
// }
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
	if (all_ate(philo) && philo->all->nb_round_eat == philo->all->args.number_of_times_each_philosopher_must_eat)
		return (0);
	pthread_mutex_unlock(&philo->all->eating_mutex);

    if (philo->all->args.number_of_times_each_philosopher_must_eat != -1
        && philo->all->nb_round_eat >= philo->all->args.number_of_times_each_philosopher_must_eat)
        return (0); // Ce philo a fini de manger
    return (1);
}

int	no_dead(t_philo **philo)
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    pthread_mutex_lock(&(*philo)->meal_mutex);
    long time_since_last_meal = time_diff_ms(&(*philo)->last_meal, &current_time);
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

// void *philo_routine_argc_6(void *arg)
// {
//     t_philo *philo = (t_philo *)arg;
// 	int nb_philo;
// 	int round;
	
// 	nb_philo = philo->all->args.nb_philo;
// 	round = 0;
	
//     while (round != nb_philo && !no_dead(&philo))
//     {
//         // Penser
//         print_status(&philo, "is thinking");
        
//         // Manger
//         take_forks(philo);
//         print_status(&philo, "is eating");
//         eat(philo);
//         put_forks(philo);
        
//         // Dormir
//         print_status(&philo, "is sleeping");
//         usleep(philo->all->args.time_to_sleep * 1000);
//     }
// 	return (NULL);
// }

void *philosopher_routine_argc_6(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    
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
		
		if (!eat(philo))
			break;
		
        // eat(philo);
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

int	ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
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

void take_forks(t_philo *philo)
{
    if (philo->id % 2 == 0)
    {
        // Philosophe pair : gauche puis droite
        pthread_mutex_lock(&philo->all->forks[philo->left_fork]);
        print_status(&philo, "\e[33mhas taken a fork\033[00m 🍴");
        pthread_mutex_lock(&philo->all->forks[philo->right_fork]);
        print_status(&philo, "\e[33mhas taken a fork\033[00m🍴");
    }
    else
    {
        // Philosophe impair : droite puis gauche
        pthread_mutex_lock(&philo->all->forks[philo->right_fork]);
        print_status(&philo, "\e[33mhas taken a fork\033[00m🍴");
        pthread_mutex_lock(&philo->all->forks[philo->left_fork]);
        print_status(&philo, "\e[33mhas taken a fork\033[00m 🍴");
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

void put_forks_odds(t_philo *philo)
{
    pthread_mutex_unlock(&philo->all->forks[philo->right_fork]);
    pthread_mutex_unlock(&philo->all->forks[philo->left_fork]);
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
