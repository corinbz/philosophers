/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 14:18:45 by ccraciun          #+#    #+#             */
/*   Updated: 2024/09/08 14:19:11 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	print_status(t_philosopher *philo, const char *status)
{
	long long	current_time;

	current_time = get_current_time();
	pthread_mutex_lock(philo->print_mutex);
	if (!*(philo->simulation_stop))
		printf("%lld %d %s\n", current_time, philo->id, status);
	pthread_mutex_unlock(philo->print_mutex);
}

// Implement a hierarchical resource allocation strategy
bool	try_pickup_forks(t_philosopher *philo)
{
	int	left_fork;
	int	right_fork;

	if (philo->id % 2 != 0)
		left_fork = (philo->id - 1);
	else
		left_fork = philo->id % philo->num_philosophers;
	if (philo->id % 2 != 0)
		right_fork = philo->id % philo->num_philosophers;
	else
		right_fork = philo->id - 1;
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	if (left_fork == right_fork)
	{
		printf("got here\n");
		ft_usleep(philo->time_to_die * 1000);
		print_status(philo, "died");
		*philo->simulation_stop = 1;
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
	philo->last_meal_time = eat_start;
	print_status(philo, "is eating");
	ft_usleep(philo->time_to_eat * 1000);
	philo->meals_eaten++;
	return_forks(philo);
	if (philo->meals_eaten == philo->num_times_to_eat)
		philo->is_full = true;
	return (1);
}

void	return_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (!philo)
		return (NULL);
	if (philo->id % 2 == 0)
		ft_usleep(philo->time_to_eat * 1000 / 4);
	while (!*philo->simulation_stop)
	{
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
