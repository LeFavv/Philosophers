/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 13:12:20 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/30 13:32:49 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//penser a check si ca exister avant de free
void    ft_free_all(t_all *all, t_args *args, long	*tab)
{
    free(all->philo);
	free(all->forks);
	free(all->ate);
	free(args);
	free(all);
	free(tab);
}