/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corin <corin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 10:05:47 by corin             #+#    #+#             */
/*   Updated: 2024/11/10 17:05:46 by corin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

static void	set_sim_stop_flag(t_data *data, bool state)
{
	pthread_mutex_lock(&data->stop_sim_mut);
	data->stop_simulation = state;
	pthread_mutex_unlock(&data->stop_sim_mut);
}

static bool	philo_starved(t_philo *philo)
{
	long	current_time;

	current_time = get_current_time();
	if ((current_time - philo->last_meal_time) >= philo->data->time_to_die)
	{
		write_status(philo, "died");
		set_sim_stop_flag(philo->data, true);
		pthread_mutex_unlock(&philo->last_meal_time_mut);
		return (true);
	}
	return (false);
}

static bool	full_or_dead(t_data *data)
{
	int		i;
	bool	full;

	i = 0;
	full = true;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i]->last_meal_time_mut);
		if (philo_starved(data->philos[i]))
			return (true);
		if (data->meals_to_full != -1)
			if (data->philos[i]->meals_eaten < data->meals_to_full)
				full = false;
		pthread_mutex_unlock(&data->philos[i]->last_meal_time_mut);
		i++;
	}
	if (data->meals_to_full != -1 && full)
	{
		set_sim_stop_flag(data, true);
		return (true);
	}
	return (false);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	wait_for_start(data->time_zero);
	while (1)
	{
		if (full_or_dead(data))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
