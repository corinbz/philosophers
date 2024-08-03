/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:14:50 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/03 16:30:52 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void start_simulation(t_simulation *sim)
{
	for (int i = 0; i < sim->num_philosophers; i++) {
		pthread_create(&sim->philosophers[i].thread, NULL, philosopher_routine, &sim->philosophers[i]);
	}
}

void *monitor_simulation(void *arg)
{
	t_simulation *sim;
	int i;
	
	sim = (t_simulation *)arg;
	while (!sim->simulation_stop)
	{
		for (i = 0; i < sim->num_philosophers; i++)
		{
			if (get_current_time() - sim->philosophers[i].last_meal_time > sim->time_to_die)
			{
				print_status(&sim->philosophers[i], "died");
				sim->simulation_stop = 1;
				return NULL;
			}
		}
		ft_usleep(1000);  // Sleep for 1ms to avoid busy waiting
	}
	return NULL;
}
