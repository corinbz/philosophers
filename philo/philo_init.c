/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:40:46 by ccraciun          #+#    #+#             */
/*   Updated: 2024/11/08 14:02:36 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Initializes basic properties of a philosopher
** @param philo: Pointer to philosopher structure
** @param id: Philosopher ID
** @param sim: Pointer to simulation structure
*/
static void	init_philosopher_basic(t_philosopher *philo, int id,
	t_simulation *sim)
{
	philo->id = id + 1;
	philo->meals_eaten = 0;
	philo->simulation_stop = &sim->simulation_stop;
	philo->print_mutex = &sim->print_mutex;
	philo->sim_stop_mut = &sim->sim_stop_mut;
	philo->time_zero_mut = &sim->time_zero_mut;
	philo->is_full = false;
	philo->time_zero = 0;
}

/*
** Initializes timing-related properties of a philosopher
** @param philo: Pointer to philosopher structure
** @param sim: Pointer to simulation structure
** @param start_time: Initial timestamp
*/
static void	init_philosopher_timings(t_philosopher *philo,
	t_simulation *sim, long start_time)
{
	philo->last_meal_time = start_time;
	philo->time_to_die = sim->time_to_die;
	philo->time_to_eat = sim->time_to_eat;
	philo->time_to_sleep = sim->time_to_sleep;
	philo->num_times_to_eat = sim->num_times_to_eat;
	philo->num_philosophers = sim->num_philosophers;
}

/*
** Initializes all philosophers in the simulation
** @param sim: Pointer to simulation structure
*/
void	init_philosophers(t_simulation *sim)
{
	int		i;
	long	start_time;

	start_time = get_current_time();
	i = 0;
	while (i < sim->num_philosophers)
	{
		init_philosopher_basic(&sim->philosophers[i], i, sim);
		init_philosopher_timings(&sim->philosophers[i], sim, start_time);
		set_fork_ids(sim, i);
		i++;
	}
}
