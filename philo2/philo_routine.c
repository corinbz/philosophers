/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corin <corin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 10:03:35 by corin             #+#    #+#             */
/*   Updated: 2024/11/10 17:39:10 by corin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

bool	sim_must_stop(t_data *data)
{
	bool	ret;

	pthread_mutex_lock(&data->stop_sim_mut);
	ret = data->stop_simulation;
	pthread_mutex_unlock(&data->stop_sim_mut);
	return (ret);
}

static void	*handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks_mut[philo->forks[0]]);
	write_status(philo, "has taken a fork");
	ft_usleep(philo->data->time_to_die);
	write_status(philo, "died");
	pthread_mutex_unlock(&philo->data->forks_mut[philo->forks[0]]);
	pthread_mutex_lock(&philo->data->stop_sim_mut);
	philo->data->stop_simulation = true;
	pthread_mutex_unlock(&philo->data->stop_sim_mut);
	return (NULL);
}
/*
* The philosopher thinks for a time that is half the time it would take for
* him to starve to death.
* The purpose of this function is to reduce fork contention by making the
* philosopher wait for a random amount of time before trying to take the forks
* again.
*/
static void	think(t_philo *philo, bool silent)
{
	long	thinking_time;
	long	time_since_last_meal;

	pthread_mutex_lock(&philo->last_meal_time_mut);
	time_since_last_meal = get_current_time() - philo->last_meal_time;
	thinking_time = (philo->data->time_to_die - time_since_last_meal
			- philo->data->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->last_meal_time_mut);
	if (thinking_time < 0)
		thinking_time = 0;
	if (thinking_time == 0 && silent == true)
		thinking_time = 1;
	if (thinking_time > 600)
		thinking_time = 200;
	if (silent == false)
		write_status(philo, "is thinking");
	ft_usleep(thinking_time);
}

static void	eat_sleep_repeat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks_mut[philo->forks[0]]);
	write_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks_mut[philo->forks[1]]);
	write_status(philo, "has taken a fork");
	write_status(philo, "is eating");
	pthread_mutex_lock(&philo->last_meal_time_mut);
	philo->last_meal_time = get_current_time();
	pthread_mutex_unlock(&philo->last_meal_time_mut);
	ft_usleep(philo->data->time_to_eat);
	if (!sim_must_stop(philo->data))
	{
		pthread_mutex_lock(&philo->last_meal_time_mut);
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->last_meal_time_mut);
	}
	pthread_mutex_unlock(&philo->data->forks_mut[philo->forks[0]]);
	pthread_mutex_unlock(&philo->data->forks_mut[philo->forks[1]]);
	write_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->meals_to_full == 0)
		return (NULL);
	pthread_mutex_lock(&philo->last_meal_time_mut);
	philo->last_meal_time = philo->data->time_zero;
	pthread_mutex_unlock(&philo->last_meal_time_mut);
	wait_for_start(philo->data->time_zero);
	if (philo->data->num_philos == 1)
		return (handle_single_philo(philo));
	else if (philo->id % 2)
		think(philo, true);
	while (!sim_must_stop(philo->data))
	{
		eat_sleep_repeat(philo);
		think(philo, false);
	}
	return (NULL);
}
