/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 01:44:03 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/27 13:56:30 by vafavard         ###   ########.fr       */
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
#define END_COLOR "\033[00m"

typedef struct s_philo t_philo;
typedef struct s_all t_all;

typedef struct s_args
{
	long	nb_philo;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	number_of_times_each_philosopher_must_eat;
}	t_args;


typedef struct s_all
{
	int				nb_forks;
	struct timeval	start;
	struct timeval	end;
	int	there_is_dead;
	t_args			args;
	pthread_t		*threads; //tableau de thread ?
	int				eat_same_time;
	int				eating;
	int				nb_ate; //nombre de philos qui ont mange ce tour si (revient a zero une fois qu'ils ont tous mange)
	int				nb_round_eat; //si == number_of_times_each_philosopher_must_eat ==> stop la simulation
	pthread_mutex_t			*forks; // Dans init malloc * nb_philo
	pthread_mutex_t			print_mutex;
	pthread_mutex_t			death_mutex;
	t_philo			*philo;
}	t_all;

typedef struct s_philo
{
	int	id;
	int left_fork;
	int right_fork;
	pthread_t thread;
	t_all *all;
	int	meals_eaten;
	struct timeval last_meal;
	pthread_mutex_t			meal_mutex;
}	t_philo;

typedef struct s_status
{
	int	nb_philo;
	int status;
}	t_status;

long	time_diff_ms(struct timeval *start, struct timeval *end);
int		mutex_destroy(t_all *all);
long	get_time_ms(void);
void	smart_sleep(long time_in_ms, t_all **all);
int		init_philosophers(t_all *all);
void	eat(t_philo *philo);
int		no_dead(t_philo **philo);
void	*philo_routine_argc_6(void *arg);
void	*philosopher_routine(void *arg);
int		ft_strcmp(char *s1, char *s2);
void	print_status(t_philo **philo, char *str);
void	take_forks(t_philo *philo);
int		join_threads(t_all *all);
int		create_threads(t_all *all);
void	put_forks(t_philo *philo);
void	put_forks_odds(t_philo *philo);
void	*monitor_routine(void *arg);
long	ft_atol(char *str);
int		init_struct_5(t_args *args, long *tab, int nb);


#endif