/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_mutex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:18:46 by ccraciun          #+#    #+#             */
/*   Updated: 2024/11/08 14:00:58 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Initializes mutexes for a single philosopher
** @param philosopher: Pointer to philosopher structure
** @return: 0 on success, 1 on failure
*/
static int	init_philosopher_mutexes(t_philosopher *philosopher)
{
	philosopher->last_meal_mut = ft_calloc(1, sizeof(pthread_mutex_t));
	philosopher->left_fork = ft_calloc(1, sizeof(pthread_mutex_t));
	philosopher->right_fork = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!philosopher->last_meal_mut)
		return (ft_error("Calloc failed\n"));
	if (pthread_mutex_init(philosopher->last_meal_mut, NULL) != 0)
		return (ft_error("Failed to init last_meal mutex\n"));
	if (pthread_mutex_init(philosopher->left_fork, NULL) != 0)
		return (ft_error("Failed to init left fork mutex\n"));
	if (pthread_mutex_init(philosopher->right_fork, NULL) != 0)
		return (ft_error("Failed to init right fork mutex\n"));
	return (0);
}

/*
** Destroys mutexes for cleanup during initialization error
** @param sim: Pointer to simulation structure
** @param index: Maximum index to process
** @return: 0 on success, 1 on failure
*/
static int	destroy_init_mutexes(t_simulation *sim, int index)
{
	int	i;

	i = index;
	if (pthread_mutex_destroy(&sim->print_mutex) != 0)
		return (ft_error("Failed to destroy print mutex\n"));
	if (pthread_mutex_destroy(&sim->sim_stop_mut) != 0)
		return (ft_error("Failed to destroy sim_stop_mut mutex\n"));
	if (pthread_mutex_destroy(&sim->time_zero_mut) != 0)
		return (ft_error("Failed to destroy time_zero_mut mutex\n"));
	while (i >= 0)
	{
		if (destroy_philosopher_mutexes(&sim->philosophers[i]) != 0)
			return (ft_error("Failed to destroy philo mutexes\n"));
		i--;
	}
	return (0);
}

/*
** Initializes all mutexes for the simulation
** @param sim: Pointer to simulation structure
** @return: 0 on success, 1 or 2 on failure
*/
int	init_mutexes(t_simulation *sim)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (free_sim_memory(sim), 1);
	if (pthread_mutex_init(&sim->sim_stop_mut, NULL) != 0)
		return (pthread_mutex_destroy(&sim->print_mutex),
			free_sim_memory(sim), 1);
	if (pthread_mutex_init(&sim->time_zero_mut, NULL) != 0)
		return (pthread_mutex_destroy(&sim->print_mutex),
			pthread_mutex_destroy(&sim->sim_stop_mut),
			free_sim_memory(sim), 1);
	while (i < sim->num_philosophers)
	{
		if (init_philosopher_mutexes(&sim->philosophers[i]) != 0)
			return (destroy_init_mutexes(sim, i - 1), free_sim_memory(sim), 2);
		i++;
	}
	return (0);
}
