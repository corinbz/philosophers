/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 14:18:45 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 16:03:53 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_status(t_philosopher *philo, const char *status)
{
	long long	current_time;

	pthread_mutex_lock(philo->print_mutex);
	if (!*(philo->simulation_stop))
	{
		current_time = get_current_time();
		printf("%lld %d %s\n", (current_time - philo->time_zero),
			philo->id, status);
	}
	pthread_mutex_unlock(philo->print_mutex);
}

// Implement a hierarchical resource allocation strategy
bool	try_pickup_forks(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	if (philo->left_fork_id == philo->right_fork_id)
	{
		ft_usleep(philo->time_to_die * 1000);
		print_status(philo, "died");
		pthread_mutex_lock(philo->sim_stop_mut);
		*philo->simulation_stop = 1;
		pthread_mutex_unlock(philo->sim_stop_mut);
		pthread_mutex_unlock(philo->left_fork);
		return (false);
	}
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a fork");
	return (true);
}

int	try_to_eat(t_philosopher *philo)
{
	long long	eat_start;

	if (!try_pickup_forks(philo))
		return (0);
	eat_start = get_current_time();
	pthread_mutex_lock(philo->last_meal_mut);
	philo->last_meal_time = eat_start;
	pthread_mutex_unlock(philo->last_meal_mut);
	print_status(philo, "is eating");
	ft_usleep(philo->time_to_eat * 1000);
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	if (philo->meals_eaten == philo->num_times_to_eat)
		philo->is_full = true;
	return (1);
}

void	check_philo_ready(t_philosopher *philo)
{
	while (true)
	{
		pthread_mutex_lock(philo->time_zero_mut);
		if (philo->time_zero == 0)
		{
			ft_usleep(100);
			pthread_mutex_unlock(philo->time_zero_mut);
			continue ;
		}
		pthread_mutex_unlock(philo->time_zero_mut);
		break ;
	}
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (!philo)
		return (NULL);
	check_philo_ready(philo);
	if (philo->id % 2 == 0)
		ft_usleep(philo->time_to_eat * 1000 / 4);
	while (!*philo->simulation_stop)
	{
		pthread_mutex_lock(philo->sim_stop_mut);
		if (*philo->simulation_stop)
		{
			pthread_mutex_unlock(philo->sim_stop_mut);
			return (NULL);
		}
		if (try_to_eat(philo))
		{
			if (philo->is_full)
				return (NULL);
			print_status(philo, "is sleeping");
			ft_usleep(philo->time_to_sleep * 1000);
			print_status(philo, "is thinking");
		}
	}
	return (NULL);
}
