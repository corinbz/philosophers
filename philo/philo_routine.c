/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 14:18:45 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/25 14:12:47 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Waits for simulation start time to be set
** @param philo: Pointer to philosopher structure
*/
static void	wait_for_start(t_philosopher *philo)
{
	pthread_mutex_lock(philo->time_zero_mut);
	while (philo->time_zero == 0)
	{
		pthread_mutex_unlock(philo->time_zero_mut);
		ft_usleep(100);
		pthread_mutex_lock(philo->time_zero_mut);
	}
	pthread_mutex_unlock(philo->time_zero_mut);
}

/*
** Implements initial delay for philosophers to prevent deadlock
** @param philo: Pointer to philosopher structure
*/
static void	handle_initial_delay(t_philosopher *philo)
{
	int	group;

	group = 0;
	if (philo->num_philosophers > 100)
	{
		group = philo->id % 4;
		ft_usleep(1000 * group);
	}
	else if (philo->id % 2 == 0)
		ft_usleep(1000);
}

/*
** Checks if simulation should stop
** @param philo: Pointer to philosopher structure
** @return: true if simulation should stop, false otherwise
*/
bool	check_simulation_stop(t_philosopher *philo)
{
	bool	should_stop;

	pthread_mutex_lock(philo->sim_stop_mut);
	should_stop = *philo->simulation_stop;
	pthread_mutex_unlock(philo->sim_stop_mut);
	return (should_stop);
}

/*
** Handles one cycle of philosopher actions
** @param philo: Pointer to philosopher structure
** @return: 0 if philosopher should stop, 1 if should continue
*/
static int	handle_philosopher_actions(t_philosopher *philo)
{
	bool	is_full;

	if (!try_to_eat(philo))
	{
		if (check_death_time(philo))
			return (0);
		ft_usleep(100);
		return (1);
	}
	pthread_mutex_lock(philo->last_meal_mut);
	is_full = philo->is_full;
	pthread_mutex_unlock(philo->last_meal_mut);
	if (is_full)
		return (0);
	print_status(philo, "is sleeping");
	ft_usleep(philo->time_to_sleep * 1000);
	if (!check_simulation_stop(philo))
		print_status(philo, "is thinking");
	return (1);
}

/*
** Main routine for philosopher threads
** @param arg: Void pointer to philosopher structure
** @return: NULL pointer when thread ends
*/
void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (!philo)
		return (NULL);
	wait_for_start(philo);
	handle_initial_delay(philo);
	while (!check_simulation_stop(philo))
	{
		if (!handle_philosopher_actions(philo))
			break ;
	}
	return (NULL);
}
