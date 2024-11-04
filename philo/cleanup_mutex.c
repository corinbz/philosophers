/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:27:31 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 15:10:39 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Destroys all forks mutexes
** @param sim: Pointer to simulation structure
** @param num_forks: Number of forks to destroy
** @return: 0 on success, 1 if any mutex destroy fails
*/
static int	destroy_fork_mutexes(t_simulation *sim, int num_forks)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < num_forks)
	{
		if (pthread_mutex_destroy(&sim->forks[i]) != 0)
		{
			ft_error("Failed to destroy fork mutex\n");
			status = 1;
		}
		i++;
	}
	return (status);
}

/*
** Destroys all philosopher mutexes
** @param sim: Pointer to simulation structure
** @param num_philos: Number of philosophers to process
** @return: 0 on success, 1 if any mutex destroy fails
*/
static int	destroy_all_philosopher_mutexes(t_simulation *sim, int num_philos)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < num_philos)
	{
		if (destroy_philosopher_mutexes(&sim->philosophers[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}

/*
** Destroys print and time_zero_mut mutexes
** @param sim: Pointer to simulation structure
** @return: 0 on success, 1 on failure
*/
static int	destroy_general_mutexes(t_simulation *sim)
{
	if (pthread_mutex_destroy(&sim->print_mutex) != 0)
	{
		ft_error("Failed to destroy print mutex\n");
		return (1);
	}
	if (pthread_mutex_destroy(&sim->time_zero_mut) != 0)
	{
		ft_error("Failed to destroy time_zero_mut mutex\n");
		return (1);
	}
	return (0);
}

/*
** Destroys all mutexes in simulation
** @param sim: Pointer to simulation structure
** @return: 0 on success, 1 if any mutex destroy fails
*/
int	cleanup_mutexes(t_simulation *sim)
{
	int	status;

	status = 0;
	if (destroy_general_mutexes(sim) != 0)
		status = 1;
	if (destroy_fork_mutexes(sim, sim->num_philosophers) != 0)
		status = 1;
	if (destroy_all_philosopher_mutexes(sim, sim->num_philosophers) != 0)
		status = 1;
	return (status);
}

/*
** Destroys mutexes up to a specific index during initialization
** @param sim: Pointer to simulation structure
** @param index: Maximum index to process
** @return: 0 on success, 1 if any mutex destroy fails
*/
int	destroy_mutexes_up_to(t_simulation *sim, int index)
{
	int	status;

	status = 0;
	if (destroy_general_mutexes(sim) != 0)
		status = 1;
	if (destroy_fork_mutexes(sim, index) != 0)
		status = 1;
	if (destroy_all_philosopher_mutexes(sim, index) != 0)
		status = 1;
	return (status);
}
