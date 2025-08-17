/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 01:49:13 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/17 04:57:53 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

long time_diff_ms(struct timeval *start, struct timeval *end) 
{
    return (end->tv_sec - start->tv_sec) * 1000
         + (end->tv_usec - start->tv_usec) / 1000;
}

void	*thinking_routine(t_all **all)
{
}

void	*sleep_routine(t_all **all)
{
	gettimeofday(&(*all)->end, NULL);
	printf("%s%ld %d is sleeping%s\n", YELLOW, time_diff_ms(&(*all)->start, &(*all)->end), 0, YELLOW);
	usleep((*all)->args.time_to_sleep);
}

void	*eat_routine(t_all **all)
{
	gettimeofday(&(*all)->end, NULL);
	printf("%s%ld %d is eating%s\n", BLUE, time_diff_ms(&(*all)->start, &(*all)->end), 0, BLUE);
	usleep((*all)->args.time_to_sleep);
}

int main(int argc, char **argv)
{
	t_args	*args;
	t_all	*all;
	long	*tab;
	int i = 0;

	if (argc == 5)
	{
		gettimeofday(&all->start, NULL);
		tab = malloc(sizeof(long) * 4);
		if (!tab)
			return (1);
		while (i < 4)
		{
			tab[i] = ft_atol(argv[i + 2]);
			i++;
		}
		if (!init_struct_5(&args, tab))
			return (free(tab),1);
	}
	else if (argc == 6)
	{
		gettimeofday(&all->start, NULL);
		tab = malloc(sizeof(long) * 5);
		if (!tab)
			return (1);
		while (i < 5)
		{
			tab[i] = ft_atol(argv[i + 2]);
			i++;
		}
		if (!init_struct_5(&args, tab))
			return (free(tab),1);
	}
	return (0);
}
