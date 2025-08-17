/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 03:03:55 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/17 16:35:56 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_struct_5(t_args *args, long *tab, int nb)
{
	args->nb_philo = tab[0];
	args->time_to_die = tab[1];
	args->time_to_eat = tab[2];
	args->time_to_sleep = tab[3];
	if (nb == 5)
		args->number_of_times_each_philosopher_must_eat = tab[4];
	else
		args->number_of_times_each_philosopher_must_eat = -1;
	return (1);
}