/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 13:17:50 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/10 15:01:36 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void ft_usleep(long long time)
{
    long long start;
    long long current;
    long long elapsed;

    if (time <= 0)
        return;

    start = get_current_time();
    while (1)
    {
        current = get_current_time();
        elapsed = current - start;
        if (elapsed < 0 || elapsed >= time / 1000)
            break;
        if (time - elapsed > 1000)
            usleep(1000);
        else
            usleep(100);
    }
}

long long get_current_time(void)
{
    struct timeval tv;
    long long time_in_ms;

    if (gettimeofday(&tv, NULL) != 0)
    {
        // Handle error (you might want to print an error message here)
        return -1;
    }
    time_in_ms = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
    return time_in_ms;
}