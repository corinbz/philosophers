/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 08:07:03 by corin             #+#    #+#             */
/*   Updated: 2024/11/11 12:48:58 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	wait_for_start(long time_zero)
{
	long	current_time;

	while (1)
	{
		current_time = get_current_time();
		if (current_time >= time_zero)
			break ;
	}
}

/*
** Custom sleep function using busy waiting
** @param ms: Time to sleep in miliseconds
*/
void	ft_usleep(long ms, t_data *data)
{
	long	start;
	long	target_time;

	start = get_current_time();
	target_time = start + ms;
	while (get_current_time() < target_time)
	{
		if (sim_must_stop(data))
			break ;
		usleep(100);
	}
}

/*
** Gets current time in milliseconds
** @return: Current time in milliseconds
*/
long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
