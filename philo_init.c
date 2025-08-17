/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 03:03:55 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/17 03:04:17 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_struct_5(t_args *args, long *tab)
{
	args->nb_philo = tab[0];
	args->number_of_times_each_philosopher_must_eat = tab[1];
	args->time_to_die = tab[2];
	args->time_to_eat = tab[3];
	args->time_to_sleep = tab[4];
	return (1);
}