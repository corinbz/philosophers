/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:27:31 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 15:10:39 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Writes error message to stderr and returns error status
** @param err: Error message string to write
** @return: Always returns 1 to indicate error
*/
int	ft_error(const char *err)
{
	write(2, err, ft_strlen(err));
	return (1);
}

/*
** Joins all philosopher threads up to a specified number
** @param sim: Pointer to simulation structure
** @param philos_num: Number of philosophers to join
** @return: 0 on success, 1 on failure
*/
int	clean_philo_threads(t_simulation *sim, int philos_num)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < philos_num)
	{
		if (pthread_join(sim->philosophers[i].thread, NULL) != 0)
			status = 1;
		i++;
	}
	if (status)
		return (ft_error("Failed to join threads\n"));
	return (0);
}

/*
** Frees all allocated memory in simulation structure
** @param sim: Pointer to simulation structure to free
*/
void	free_sim_memory(t_simulation *sim)
{
	if (!sim)
		return ;
	if (sim->philosophers)
		free(sim->philosophers);
	if (sim->forks)
		free(sim->forks);
	free(sim);
}

/*
** Destroys all mutexes for a single philosopher
** @param philo: Pointer to philosopher structure
** @return: 0 on success, 1 if any mutex destroy fails
*/
int	destroy_philosopher_mutexes(t_philosopher *philo)
{
	int	status;

	status = 0;
	if (philo->time_zero_mut)
	{
		pthread_mutex_destroy(philo->time_zero_mut);
		free(philo->time_zero_mut);
	}
	if (philo->last_meal_mut)
	{
		pthread_mutex_destroy(philo->last_meal_mut);
		free(philo->last_meal_mut);
	}
	if (philo->sim_stop_mut)
	{
		pthread_mutex_destroy(philo->sim_stop_mut);
		free(philo->sim_stop_mut);
	}
	return (status);
}

/*
** Sets simulation stop flag and destroys all mutexes
** @param sim: Pointer to simulation structure
*/
void	cleanup_simulation(t_simulation *sim)
{
	int	i;

	if (!sim)
		return ;
	pthread_mutex_lock(sim->philosophers[0].sim_stop_mut);
	sim->simulation_stop = true;
	pthread_mutex_unlock(sim->philosophers[0].sim_stop_mut);
	i = 0;
	while (i < sim->num_philosophers)
	{
		pthread_join(sim->philosophers[i].thread, NULL);
		i++;
	}
	cleanup_mutexes(sim);
	free_sim_memory(sim);
}
