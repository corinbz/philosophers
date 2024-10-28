/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:40:46 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/25 14:10:15 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Allocates memory for philosophers and forks arrays
** @param sim: Pointer to simulation structure
** @return: 1 on success, 0 on failure
*/
static int	allocate_resources(t_simulation *sim)
{
	sim->philosophers = malloc(sizeof(t_philosopher) * sim->num_philosophers);
	if (!sim->philosophers)
		return (0);
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philosophers);
	if (!sim->forks)
	{
		free(sim->philosophers);
		return (0);
	}
	return (1);
}

/*
** Sets up time synchronization for all philosophers
** @param sim: Pointer to simulation structure
*/
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

/*
** Creates philosopher threads and starts simulation
** @param sim: Pointer to simulation structure
** @return: 0 on success, non-zero on failure
*/
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
		if (sim->num_philosophers > 100)
			ft_usleep(50);
	}
	if (err)
		return (clean_philo_threads(sim, i));
	set_starting_time(sim);
	return (0);
}

/*
** Main initialization function for the simulation
** @param ac: Argument count
** @param av: Argument values
** @return: Pointer to initialized simulation or NULL on failure
*/
t_simulation	*init_simulation(int ac, char **av)
{
	t_simulation	*sim;
	int				init_mutex;

	if (ac != 5 && ac != 6)
		return (ft_error("Invalid number of arguments\n"), NULL);
	if (!valid_args(av))
		return (ft_error("Arguments must be positive digits only\n"), NULL);
	sim = ft_calloc(1, sizeof(t_simulation));
	if (!sim)
		return (ft_error("Memory allocation failed\n"), NULL);
	if (!parse_arguments(sim, ac, av))
		return (free(sim), ft_error("Invalid argument values\n"), NULL);
	if (!allocate_resources(sim))
		return (free(sim), ft_error("Failed to allocate resources\n"), NULL);
	init_mutex = init_mutexes(sim);
	if (init_mutex > 0)
		return (free(sim), ft_error("Failed to initialize mutexes\n"), NULL);
	init_philosophers(sim);
	if (start_simulation(sim) != 0)
	{
		cleanup_simulation(sim);
		return (ft_error("Failed to start simulation\n"), NULL);
	}
	return (sim);
}

/*
** Helper function to set fork IDs for a philosopher
** @param sim: Pointer to simulation structure
** @param i: Index of philosopher
*/
void	set_fork_ids(t_simulation *sim, int i)
{
	if (sim->num_philosophers == 1)
	{
		sim->philosophers[i].left_fork_id = 0;
		sim->philosophers[i].right_fork_id = 0;
		return ;
	}
	if (i % 2 == 0)
	{
		sim->philosophers[i].left_fork_id = i;
		sim->philosophers[i].right_fork_id = (i + 1) % sim->num_philosophers;
	}
	else
	{
		sim->philosophers[i].left_fork_id = (i + 1) % sim->num_philosophers;
		sim->philosophers[i].right_fork_id = i;
	}
}