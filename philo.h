/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 01:44:03 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/17 05:02:36 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

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

typedef struct s_all
{
	int				nb_forks;
	struct timeval	start;
	struct timeval	end;
	t_args			args;
	int				nb_forks;
	int				nb_ate; //nombre de philos qui ont mange ce tour si (revient a zero une fois qu'ils ont tous mange)
	int				nb_round_eat; //si == number_of_times_each_philosopher_must_eat ==> stop la simulation
}	t_all;

#endif