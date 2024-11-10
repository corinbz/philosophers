/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corin <corin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 08:07:03 by corin             #+#    #+#             */
/*   Updated: 2024/11/10 16:19:30 by corin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	wait_for_start(long time_zero)
{
	while (get_current_time() < time_zero)
		usleep(100);
}

/*
** Custom sleep function using busy waiting
** @param ms: Time to sleep in miliseconds
*/
void	ft_usleep(long ms)
{
	long	start;
	long	target_time;

	start = get_current_time();
	target_time = start + ms;
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
