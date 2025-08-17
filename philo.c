/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 01:49:13 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/17 17:13:25 by vafavard         ###   ########.fr       */
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
void	print_status(t_philo *philo, char *str);
void	take_forks(t_philo *philo);
int		join_threads(t_all *all);
int		create_threads(t_all *all);
void	put_forks(t_philo *philo);
void	can_eat_same_time(t_all **all);

//chaque philo doit etre un thread
//bien penser a tout init
//mutex ? 
//lock quand on fait une action
//int pthread_join(pthread_t thread, void **retval); Permet d’attendre la fin d’un thread (bloquant).
// int pthread_create(pthread_t *restrict thread,
//                           const pthread_attr_t *restrict attr,
//                           void *(*start_routine)(void *),
//                           void *restrict arg);
//int pthread_detach(pthread_t thread);
//pthread_mutex_t    mutex;
//int pthread_mutex_init(pthread_mutex_t *mutex,
//                        const pthread_mutexattr_t *mutexattr);
// int pthread_mutex_lock(pthread_mutex_t *mutex); // Verrouillage
// int pthread_mutex_unlock(pthread_mutex_t *mutex); // Déverrouillage
//int pthread_mutex_destroy(pthread_mutex_t *mutex);
//int usleep(useconds_t microseconds);

//si nb_philo / 2 == 0
//la moitie des philos peuvent manger en meme temps

//si nb_philo % 2 == 1
//nb_philo - 1 / 2 peuvent manger en meme temps

//l'etat thinking n'est pas obligatoire
//c'est juste le moment d'attente entre sleep et eat

//simulation stop si un philo meurt ou si les philos on mange n fois necessaires (derniers argument main)

//mutex lock et unlock retournent 0 en cas de reussite
//impair droite ??
//pari gauche ??


long time_diff_ms(struct timeval *start, struct timeval *end) 
{
    return (end->tv_sec - start->tv_sec) * 1000
         + (end->tv_usec - start->tv_usec) / 1000;
}

// void	sleep_routine(t_all **all)
// {
// 	gettimeofday(&(*all)->end, NULL);
// 	printf("%s%ld %d is sleeping%s\n", YELLOW, time_diff_ms(&(*all)->start, &(*all)->end), 0, YELLOW);
// 	usleep((*all)->args.time_to_sleep);
// }

// void	*eat_routine(t_all **all)
// {
// 	gettimeofday(&(*all)->end, NULL);
// 	printf("%s%ld %d is eating%s\n", BLUE, time_diff_ms(&(*all)->start, &(*all)->end), 0, BLUE);
// 	usleep((*all)->args.time_to_sleep);
// }

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
	 while (i < all->args.nb_philo)
    {
        all->philo[i].id = i;
        all->philo[i].left_fork = i;
        all->philo[i].right_fork = (i + 1) % all->args.nb_philo;
        all->philo[i].all = all;
        all->philo[i].meals_eaten = 0;
        i++;
    }
	return (1);
}

void	eat(t_philo *philo)
{
	philo->meals_eaten += 1;
	usleep(philo->all->args.time_to_sleep);
	gettimeofday(&philo->last_meal, NULL); //a savoir comment s'en servir et ne pas rater si un philo doit mourir de faim
}

void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    // while (all_ate(philo) || there_is_dead(philo->all))
    while (1)
    {
        // Penser
        print_status(philo, "is thinking");
        
        // Manger
        take_forks(philo);
        print_status(philo, "is eating");
        eat(philo);
        put_forks(philo);
        
        // Dormir
        print_status(philo, "is sleeping");
        usleep(philo->all->args.time_to_sleep);
    }
    return (NULL);
}

void	print_status(t_philo *philo, char *str)
{
	long time;
	
	gettimeofday(&philo->all->end, NULL);
	time = time_diff_ms(&philo->all->start, &philo->all->end);
	printf("%ld %d %s\n", time, philo->id, str);
}

void take_forks(t_philo *philo)
{
    // Pour éviter les deadlocks, toujours prendre la fourchette 
    // avec l'index le plus petit en premier
    if (philo->left_fork < philo->right_fork)
    {
        pthread_mutex_lock(&philo->all->forks[philo->left_fork]);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(&philo->all->forks[philo->right_fork]);
        print_status(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(&philo->all->forks[philo->right_fork]);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(&philo->all->forks[philo->left_fork]);
        print_status(philo, "has taken a fork");
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
		// Allocation des structures
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
		join_threads(all);    // Attendre que les threads finissent
		mutex_destroy(all);   // Détruire les mutex
		free(all->philo);     // Libérer les philosophes
		free(all->forks);     // Libérer les fourchettes
		free(all);
	}
	return (0);
}
