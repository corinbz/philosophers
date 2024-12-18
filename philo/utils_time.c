/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 13:17:50 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 09:49:12 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Custom sleep function using busy waiting
** @param microseconds: Time to sleep in microseconds
*/
void	ft_usleep(long microseconds)
{
	long	start;
	long	target_time;

	start = get_current_time();
	target_time = start + (microseconds / 1000);
	while (get_current_time() < target_time)
		usleep(100);
}

/*
** Gets current time in milliseconds
** @return: Current time in milliseconds
*/
long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}
