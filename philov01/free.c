/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 13:12:20 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/30 16:50:05 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//penser a check si ca exister avant de free
void ft_free_all(t_all *all, t_args *args, long *tab)
{
    if (all)
    {
        if (all->philo)
            free(all->philo);
        if (all->forks)
            free(all->forks);
        if (all->ate)
            free(all->ate);
        free(all);
    }
    if (tab)
        free(tab);
    if (args)
        free(args);
}
