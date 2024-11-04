/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 14:18:45 by ccraciun          #+#    #+#             */
/*   Updated: 2024/11/01 11:50:20 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Prints philosopher status with timestamp
** @param philo: Pointer to philosopher structure
** @param status: Status message to print
*/
void	print_status(t_philosopher *philo, const char *status)
{
	bool	should_stop;
	long	current_time;

	pthread_mutex_lock(philo->sim_stop_mut);
	should_stop = *philo->simulation_stop;
	pthread_mutex_unlock(philo->sim_stop_mut);
	if (should_stop)
		return ;
	pthread_mutex_lock(philo->print_mutex);
	if (!check_simulation_stop(philo))
	{
		current_time = get_current_time();
		printf("%ld %d %s\n",
			(current_time - philo->time_zero),
			philo->id,
			status);
	}
	pthread_mutex_unlock(philo->print_mutex);
}

/*
** Checks if philosopher has exceeded time to die
** @param philo: Pointer to philosopher structure
** @return: true if philosopher should die, false otherwise
*/
// bool	check_death_time(t_philosopher *philo)
// {
// 	long	current_time;
// 	long	last_meal;

// 	pthread_mutex_lock(philo->last_meal_mut);
// 	current_time = get_current_time();
// 	last_meal = current_time - philo->last_meal_time;
// 	pthread_mutex_unlock(philo->last_meal_mut);
// 	return (last_meal >= philo->time_to_die);
// }

/*
** Updates philosopher's meal count and full status
** @param philo: Pointer to philosopher structure
*/
void	update_meal_status(t_philosopher *philo)
{
	pthread_mutex_lock(philo->last_meal_mut);
	philo->meals_eaten++;
	if (philo->num_times_to_eat != -1
		&& philo->meals_eaten >= philo->num_times_to_eat)
		philo->is_full = true;
	pthread_mutex_unlock(philo->last_meal_mut);
}

/*
** Checks if philosopher has eaten required number of times
** @param philo: Pointer to philosopher structure
** @return: true if philosopher is full, false otherwise
*/
bool	check_philosopher_full(t_philosopher *philo)
{
	bool	is_full;

	pthread_mutex_lock(philo->last_meal_mut);
	is_full = philo->is_full;
	pthread_mutex_unlock(philo->last_meal_mut);
	return (is_full);
}
