/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 14:18:45 by ccraciun          #+#    #+#             */
/*   Updated: 2024/11/08 10:11:06 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Handles special case for a single philosopher
** @param philo: Pointer to philosopher structure
** @return: Always returns false as single philosopher can't eat
*/
static bool	handle_single_philosopher(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	ft_usleep(philo->time_to_die * 1000);
	print_status(philo, "died");
	pthread_mutex_lock(philo->sim_stop_mut);
	*philo->simulation_stop = true;
	pthread_mutex_unlock(philo->sim_stop_mut);
	pthread_mutex_unlock(philo->left_fork);
	return (false);
}

/*
** Sets up fork order to prevent deadlock
** @param philo: Pointer to philosopher structure
** @param first: Pointer to first fork mutex pointer
** @param second: Pointer to second fork mutex pointer
*/
static void	setup_fork_order(t_philosopher *philo,
	pthread_mutex_t **first, pthread_mutex_t **second)
{
	if (philo->left_fork_id < philo->right_fork_id)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
}

/*
** Releases forks after philosopher finishes eating
** @param philo: Pointer to philosopher structure
*/
static void	release_forks(t_philosopher *philo)
{
	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

/*
** Attempts to pick up both forks
** @param philo: Pointer to philosopher structure
** @return: true if both forks acquired, false otherwise
*/
bool	try_pickup_forks(t_philosopher *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->num_philosophers == 1)
		return (handle_single_philosopher(philo));
	setup_fork_order(philo, &first_fork, &second_fork);
	if (check_simulation_stop(philo))
		return (false);
	if (pthread_mutex_lock(first_fork) != 0)
		return (false);
	print_status(philo, "has taken a fork");
	if (check_simulation_stop(philo))
	{
		pthread_mutex_unlock(first_fork);
		return (false);
	}
	if (pthread_mutex_lock(second_fork) != 0)
	{
		pthread_mutex_unlock(first_fork);
		return (false);
	}
	print_status(philo, "has taken a fork");
	return (true);
}

/*
** Handles the eating action for a philosopher
** @param philo: Pointer to philosopher structure
** @return: 1 if eating successful, 0 if failed to eat
*/
int	try_to_eat(t_philosopher *philo)
{
	if (!try_pickup_forks(philo))
		return (0);
	pthread_mutex_lock(philo->last_meal_mut);
	philo->last_meal_time = get_current_time();
	print_status(philo, "is eating");
	ft_usleep(philo->time_to_eat * 1000);
	release_forks(philo);
	pthread_mutex_unlock(philo->last_meal_mut);
	update_meal_status(philo);
	return (1);
}
