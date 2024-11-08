/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:47:27 by ccraciun          #+#    #+#             */
/*   Updated: 2024/11/08 09:56:48 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	set_starting_time(t_simulation *sim)
{
	int		i;
	long	start_time;

	start_time = get_current_time();
	i = 0;
	while (i < sim->num_philosophers)
	{
		pthread_mutex_lock(sim->philosophers[i].time_zero_mut);
		pthread_mutex_lock(sim->philosophers[i].last_meal_mut);
		i++;
	}
	i = 0;
	while (i < sim->num_philosophers)
	{
		sim->philosophers[i].time_zero = start_time;
		sim->philosophers[i].last_meal_time = start_time;
		i++;
	}
	i--;
	while (i >= 0)
	{
		pthread_mutex_unlock(sim->philosophers[i].last_meal_mut);
		pthread_mutex_unlock(sim->philosophers[i].time_zero_mut);
		i--;
	}
}

int	start_simulation(t_simulation *sim)
{
	int	i;
	int	err;

	i = 0;
	err = 0;
	while (i < sim->num_philosophers)
	{
		if (pthread_create(&sim->philosophers[i].thread,
				NULL, philosopher_routine, &sim->philosophers[i]) != 0)
		{
			sim->simulation_stop = true;
			err = 1;
			break ;
		}
		i++;
	}
	if (err)
		return (clean_philo_threads(sim, i));
	set_starting_time(sim);
	return (0);
}
