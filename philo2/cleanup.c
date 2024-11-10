/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corin <corin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 10:11:38 by corin             #+#    #+#             */
/*   Updated: 2024/11/10 16:57:11 by corin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	*free_data(t_data *data)
{
	int	i;

	i = 0;
	if (!data)
		return (NULL);
	if (data->forks_mut)
		free_forks_mut(data);
	if (data->philos)
	{
		while (i < data->num_philos)
		{
			if (data->philos[i])
			{
				pthread_mutex_destroy(&data->philos[i]->last_meal_time_mut);
				free(data->philos[i]);
			}
			i++;
		}
		free(data->philos);
	}
	free(data);
	return (NULL);
}

void	clean_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i]->philo_thread, NULL);
		i++;
	}
	if (data->num_philos > 1)
		pthread_join(data->monitor_thread, NULL);
	pthread_mutex_destroy(&data->print_mut);
	pthread_mutex_destroy(&data->stop_sim_mut);
	free_data(data);
}
