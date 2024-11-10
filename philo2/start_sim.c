/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_sim.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corin <corin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 10:01:57 by corin             #+#    #+#             */
/*   Updated: 2024/11/10 15:25:54 by corin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

bool	start_sim(t_data *data)
{
	int	i;

	i = 0;
	data->time_zero = get_current_time() + (data->num_philos * 2 * 10);
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i]->philo_thread, NULL,
				&philo_routine, data->philos[i]) != 0)
		{
			display_message("Error: Thread creation failed\n", false);
			return (false);
		}
		i++;
	}
	if (data->num_philos > 1)
	{
		if (pthread_create(&data->monitor_thread, NULL,
				&monitor_routine, data) != 0)
		{
			display_message("Error: Thread creation failed\n", false);
			return (false);
		}
	}
	return (true);
}
