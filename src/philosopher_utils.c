/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:15:58 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/10 14:58:57 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void print_status(t_philosopher *philo, const char *status)
{
    pthread_mutex_lock(philo->print_mutex);
    if (!*(philo->simulation_stop))
        printf("%lld %d %s\n", get_current_time(), philo->id, status);
    pthread_mutex_unlock(philo->print_mutex);
}

bool try_pickup_forks(t_philosopher *philo)
{
    int left_fork = philo->id - 1;
    int right_fork = philo->id % philo->num_philosophers;

    pthread_mutex_lock(philo->waiter_mutex);
    if (philo->forks_available[left_fork] && philo->forks_available[right_fork])
    {
        philo->forks_available[left_fork] = false;
        philo->forks_available[right_fork] = false;
        pthread_mutex_unlock(philo->waiter_mutex);

        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");
        return true;
    }
    pthread_mutex_unlock(philo->waiter_mutex);
    return false;
}

void return_forks(t_philosopher *philo)
{
    int left_fork = philo->id - 1;
    int right_fork = philo->id % philo->num_philosophers;

    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(philo->left_fork);

    pthread_mutex_lock(philo->waiter_mutex);
    philo->forks_available[left_fork] = true;
    philo->forks_available[right_fork] = true;
    pthread_mutex_unlock(philo->waiter_mutex);
}

int try_to_eat(t_philosopher *philo)
{
    if (!try_pickup_forks(philo))
        return 0;

    if (*(philo->simulation_stop))
    {
        return_forks(philo);
        return 0;
    }

    print_status(philo, "is eating");
    philo->last_meal_time = get_current_time();
    ft_usleep(philo->time_to_eat * 1000);
    philo->meals_eaten++;

    return_forks(philo);
    return 1;
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    
    if (!philo) return NULL;
    
    while (!*(philo->simulation_stop))
    {
        if (!try_to_eat(philo))
        {
            ft_usleep(1000);  // Small delay before trying again
            continue;
        }
        if (*(philo->simulation_stop)) break;
        print_status(philo, "is sleeping");
        ft_usleep(philo->time_to_sleep * 1000);
        print_status(philo, "is thinking");
    }
    return NULL;
}