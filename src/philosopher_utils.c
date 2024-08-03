/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:15:58 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/03 16:33:51 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void print_status(t_philosopher *philo, const char *status)
{
	
	pthread_mutex_lock(philo->print_mutex);
	printf("%lld %d %s\n", get_current_time(), philo->id, status);
	pthread_mutex_unlock(philo->print_mutex);
}

void pickup_forks(t_philosopher *philo)
{
    pthread_mutex_lock(philo->left_fork);
    print_status(philo, "has taken a fork");
    if (philo->left_fork == philo->right_fork)
    {
        // If there's only one fork, just wait until death
        while (!(*(philo->simulation_stop)))
        {
            ft_usleep(1000); // Sleep for 1ms and check again
        }
        pthread_mutex_unlock(philo->left_fork);
        return;
    }
    pthread_mutex_lock(philo->right_fork);
    print_status(philo, "has taken a fork");
}

void return_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    
    while (!(*(philo->simulation_stop)))
    {
        print_status(philo, "is thinking");
        
        if (*(philo->simulation_stop)) break;

        pickup_forks(philo);
        if (*(philo->simulation_stop))
        {
            if (philo->left_fork != philo->right_fork)
            {
                pthread_mutex_unlock(philo->right_fork);
            }
            pthread_mutex_unlock(philo->left_fork);
            break;
        }

        if (philo->left_fork != philo->right_fork)
        {
            print_status(philo, "is eating");
            philo->last_meal_time = get_current_time();
            ft_usleep(philo->time_to_eat * 1000);
            philo->meals_eaten++;
        }

        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
        
        if (*(philo->simulation_stop)) break;

        print_status(philo, "is sleeping");
        ft_usleep(philo->time_to_sleep * 1000);
    }
    return (NULL);
}