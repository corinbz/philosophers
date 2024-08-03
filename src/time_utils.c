/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 13:17:50 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/03 15:36:41 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

// Improved version of sleep function
void ft_usleep(long long time)
{
    long long start;
    long long elapsed;

    start = get_current_time();
    while (1)
    {
        elapsed = get_current_time() - start;
        if (elapsed >= time / 1000)
            break;
        usleep(100);  // Sleep in short intervals
    }
}
// Gets the current time in milliseconds
long long get_current_time()
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
}