/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 02:11:58 by vafavard          #+#    #+#             */
/*   Updated: 2025/09/03 13:56:00 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	time_diff_ms(struct timeval *start, struct timeval *end);
long	get_time_ms(void);
void	smart_sleep(long time_in_ms, t_all **all);

long	time_diff_ms(struct timeval *start, struct timeval *end)
{
	long	seconds;
	long	microseconds;

	seconds = end->tv_sec - start->tv_sec;
	microseconds = end->tv_usec - start->tv_usec;
	return ((seconds * 1000) + (microseconds / 1000));
}

long	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(long time_in_ms, t_all **all)
{
	struct timeval	start;
	struct timeval	current;
	long			elapsed;
	int				dead;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&current, NULL);
		elapsed = time_diff_ms(&start, &current);
		if (elapsed >= time_in_ms)
			break ;
		pthread_mutex_lock(&(*all)->death_mutex);
		dead = (*all)->there_is_dead;
		pthread_mutex_unlock(&(*all)->death_mutex);
		if (dead)
			break ;
		if (time_in_ms - elapsed > 1)
			usleep(500);
		else
			usleep(50);
	}
}
