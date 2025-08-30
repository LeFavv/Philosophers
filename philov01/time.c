/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 02:11:58 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/30 02:14:05 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long time_diff_ms(struct timeval *start, struct timeval *end);
long	get_time_ms(void);
void smart_sleep(long time_in_ms, t_all **all);


long time_diff_ms(struct timeval *start, struct timeval *end)
{
    long seconds = end->tv_sec - start->tv_sec;
    long microseconds = end->tv_usec - start->tv_usec;
    return (seconds * 1000) + (microseconds / 1000);
}

  long	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void smart_sleep(long time_in_ms, t_all **all)
{
    long start_time = get_time_ms();

    while (get_time_ms() - start_time < time_in_ms)
    {
        pthread_mutex_lock(&(*all)->death_mutex);
        int dead = (*all)->there_is_dead;
        pthread_mutex_unlock(&(*all)->death_mutex);

        if (dead)
            break;
        usleep(200);
    }
}
