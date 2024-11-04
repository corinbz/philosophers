/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_status_check.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 14:18:45 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/28 14:12:47 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Checks if a philosopher has eaten their required meals
** @param philo: Pointer to philosopher structure
** @return: true if philosopher is full, false otherwise
*/
bool	is_philosopher_full(t_philosopher *philo)
{
	bool	full_status;

	pthread_mutex_lock(philo->last_meal_mut);
	full_status = philo->is_full;
	pthread_mutex_unlock(philo->last_meal_mut);
	return (full_status);
}

/*
** Updates simulation status when all philosophers are full
** @param sim: Pointer to simulation structure
*/
static void	handle_all_full(t_simulation *sim)
{
	pthread_mutex_lock(sim->philosophers[0].sim_stop_mut);
	if (!sim->simulation_stop)
	{
		pthread_mutex_lock(&sim->print_mutex);
		printf("All philosophers have eaten enough times\n");
		sim->simulation_stop = true;
		pthread_mutex_unlock(&sim->print_mutex);
	}
	pthread_mutex_unlock(sim->philosophers[0].sim_stop_mut);
}

/*
** Determines if all philosophers have eaten their required meals
** @param sim: Pointer to simulation structure
** @return: true if all philosophers are full, false otherwise
*/
bool	check_all_philosophers_full(t_simulation *sim)
{
	int		i;
	bool	all_full;

	all_full = true;
	i = 0;
	while (i < sim->num_philosophers)
	{
		if (!is_philosopher_full(&sim->philosophers[i]))
		{
			all_full = false;
			break ;
		}
		i++;
	}
	if (all_full)
		handle_all_full(sim);
	return (all_full);
}
