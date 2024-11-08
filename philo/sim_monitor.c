/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_monitor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:47:27 by ccraciun          #+#    #+#             */
/*   Updated: 2024/11/08 09:54:52 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Handles philosopher death event
** @param sim: Pointer to simulation structure
** @param i: Index of dead philosopher
*/
// static void	handle_death(t_simulation *sim, int i, long current_time)
// {
// 	pthread_mutex_lock(&sim->sim_stop_mut);
// 	pthread_mutex_lock(&sim->print_mutex);
// 	if (!sim->simulation_stop)
// 	{
// 		printf("%ld %d died\n",
// 			current_time - sim->philosophers[i].time_zero,
// 			sim->philosophers[i].id);
// 		sim->simulation_stop = true;
// 	}
// 	pthread_mutex_unlock(&sim->print_mutex);
// 	pthread_mutex_unlock(&sim->sim_stop_mut);
// }

/*
** Checks health status of a single philosopher
** @param sim: Pointer to simulation structure
** @param i: Index of philosopher to check
** @return: true if philosopher is alive, false if dead
*/
static bool	check_philosopher_health(t_simulation *sim, int i)
{
	long	current_time;
	long	ate_last;
	bool	is_full;

	pthread_mutex_lock(sim->philosophers[i].last_meal_mut);
	is_full = sim->philosophers[i].is_full;
	if (!is_full)
	{
		current_time = get_current_time();
		ate_last = current_time - sim->philosophers[i].last_meal_time;
		if (ate_last > sim->time_to_die)
		{
			pthread_mutex_unlock(sim->philosophers[i].last_meal_mut);
			print_status(&sim->philosophers[i],"died");
			return (false);
		}
	}
	pthread_mutex_unlock(sim->philosophers[i].last_meal_mut);
	return (true);
}

/*
** Checks health of all philosophers
** @param sim: Pointer to simulation structure
** @return: true if all philosophers are alive, false if any died
*/
static bool	monitor_philosophers_health(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philosophers)
	{
		if (!check_philosopher_health(sim, i))
			return (false);
		i++;
	}
	return (true);
}

/*
** Checks if simulation should stop
** @param sim: Pointer to simulation structure
** @return: true if simulation should stop, false otherwise
*/
static bool	should_stop_simulation(t_simulation *sim)
{
	bool	should_stop;

	pthread_mutex_lock(sim->philosophers[0].sim_stop_mut);
	should_stop = sim->simulation_stop;
	pthread_mutex_unlock(sim->philosophers[0].sim_stop_mut);
	return (should_stop);
}

/*
** Main monitoring routine that checks philosophers' status
** @param arg: Void pointer to simulation structure
** @return: NULL pointer when thread ends
*/
void	*monitor_simulation(void *arg)
{
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	if (!sim || !sim->philosophers)
		return (NULL);
	ft_usleep(sim->time_to_die * 500);
	while (1)
	{
		if (should_stop_simulation(sim))
			return (NULL);
		if (!monitor_philosophers_health(sim))
			return (NULL);
		if (check_all_philosophers_full(sim))
			return (NULL);
		ft_usleep(sim->time_to_die * 300);
	}
	return (NULL);
}
