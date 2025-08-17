/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 01:44:03 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/17 15:14:13 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

#define EATING 1
#define THINKING 2
#define SLEEPING 3
#define DEAD 4

#define YELLOW "\e[33m"
#define RED "\e[31m"
#define GREEN "\e[32m"
#define	BLUE "\e[34m"

typedef struct s_args
{
	long	nb_philo;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	number_of_times_each_philosopher_must_eat;
}	t_args;

typedef struct s_philo
{
	int	id;
	int left_fork;
	int right_fork;
	pthread_t thread;
	t_all *all;
	int	meals_eaten;
	struct timeval last_meal;
}	t_philo;

typedef struct s_all
{
	int				nb_forks;
	struct timeval	start;
	struct timeval	end;
	t_args			args;
	pthread_t		*threads; //tableau de thread ?
	int				nb_forks; // == nb_philo
	int				eat_same_time;
	int				eating;
	int				nb_ate; //nombre de philos qui ont mange ce tour si (revient a zero une fois qu'ils ont tous mange)
	int				nb_round_eat; //si == number_of_times_each_philosopher_must_eat ==> stop la simulation
	pthread_mutex_t			*forks; // Dans init malloc * nb_philo
	pthread_mutex_t			print_mutex;
	t_philo	*philo;
}	t_all;


typedef struct s_status
{
	int	nb_philo;
	int status;
}	t_status;

#endif