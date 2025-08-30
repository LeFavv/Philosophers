/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 01:44:03 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/30 17:25:14 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <string.h>

#define YELLOW "\e[33m"
#define RED "\e[31m"
#define GREEN "\e[32m"
#define	BLUE "\e[34m"
#define END_COLOR "\033[00m"

#define ATE 1
#define NEED_TO_EAT 0

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
	pthread_t		*threads;
	int				eat_same_time; //useless
	int				eating; //useless
	int				*ate;
	int				nb_ate; //useless
	int				nb_round_eat;
	pthread_mutex_t			*forks;
	pthread_mutex_t			print_mutex;
	pthread_mutex_t			death_mutex;
	pthread_mutex_t			eating_mutex;
	t_philo			*philo;
}	t_all;

typedef struct s_philo
{
	int	id;
	int left_fork;
	int right_fork;
	pthread_t thread;
	t_all *all;
	int	meals_eaten; //useless
	struct timeval last_meal;
	pthread_mutex_t			meal_mutex;
}	t_philo;

typedef struct s_status
{
	int	nb_philo; 
	int status;
}	t_status;

//temporaire
void    ft_free_all(t_all *all, t_args *args, long	*tab);

//eat
int		all_ate(t_philo *philo);
int		eat(t_philo *philo);
int		no_dead(t_philo **philo);

//fork
void	take_forks(t_philo *philo);
void	put_forks(t_philo *philo);
void	put_forks_odds(t_philo *philo);

//philo_init
int		init_struct_5(t_args *args, long *tab, int nb);
int		init_philosophers(t_all *all);
void	init_ate(t_all **all);
int		mutex_init(t_all *all);
int		mutex_init_2(t_all *all);

//philo_utils
long	ft_atol(char *str);
int		ft_strcmp(char *s1, char *s2);

//print
void    print_status(t_philo **philo, char *str);
void    print_status_6(t_philo **philo, char *str);

//routine
void	*philosopher_routine_argc_6(void *arg);
void	*philosopher_routine(void *arg);
void    *monitor_routine(void *arg);
void	last_meal(t_philo *philo);
void	put_the_right_fork(t_philo *philo);

//thread_and_mutex
int		mutex_destroy(t_all *all);
int		join_threads(t_all *all);
int 	create_threads(t_all *all);

//time
long	time_diff_ms(struct timeval *start, struct timeval *end);
long	get_time_ms(void);
void	smart_sleep(long time_in_ms, t_all **all);


#endif