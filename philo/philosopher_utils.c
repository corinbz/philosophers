/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 14:18:45 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/25 14:12:47 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool    check_simulation_stop(t_philosopher *philo)
{
    bool should_stop;

    pthread_mutex_lock(philo->sim_stop_mut);
    should_stop = *philo->simulation_stop;
    pthread_mutex_unlock(philo->sim_stop_mut);
    return (should_stop);
}

void    print_status(t_philosopher *philo, const char *status)
{
    long    current_time;

    pthread_mutex_lock(philo->print_mutex);
    if (!check_simulation_stop(philo))
    {
        current_time = get_current_time();
        printf("%ld %d %s\n", (current_time - philo->time_zero),
            philo->id, status);
    }
    pthread_mutex_unlock(philo->print_mutex);
}

bool    check_death_time(t_philosopher *philo)
{
    long long current_time;
    long long last_meal;

    pthread_mutex_lock(philo->last_meal_mut);
    current_time = get_current_time();
    last_meal = current_time - philo->last_meal_time;
    pthread_mutex_unlock(philo->last_meal_mut);

    // More aggressive threshold for large groups
    return (last_meal > (philo->time_to_die -
        (philo->num_philosophers > 50 ? 200 : 100)));
}

bool    try_pickup_forks(t_philosopher *philo)
{
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    if (philo->num_philosophers == 1)
    {
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
        while (!check_simulation_stop(philo))
            ft_usleep(1000);
        pthread_mutex_unlock(philo->left_fork);
        return (false);
    }

    // For large numbers, use fixed fork order based on IDs to prevent deadlock
    if (philo->id % 2 == 0)
    {
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }
    else
    {
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    }

    if (pthread_mutex_lock(first_fork) != 0)
        return (false);
    print_status(philo, "has taken a fork");

    if (pthread_mutex_lock(second_fork) != 0)
    {
        pthread_mutex_unlock(first_fork);
        return (false);
    }
    print_status(philo, "has taken a fork");

    return (true);
}


int    try_to_eat(t_philosopher *philo)
{
    if (!try_pickup_forks(philo))
        return (0);

    // Update last meal time as soon as forks are acquired
    pthread_mutex_lock(philo->last_meal_mut);
    philo->last_meal_time = get_current_time();
    pthread_mutex_unlock(philo->last_meal_mut);

    print_status(philo, "is eating");
    ft_usleep(philo->time_to_eat * 1000);

    pthread_mutex_lock(philo->last_meal_mut);
    philo->meals_eaten++;
    if (philo->num_times_to_eat != -1 && philo->meals_eaten >= philo->num_times_to_eat)
        philo->is_full = true;
    pthread_mutex_unlock(philo->last_meal_mut);

    // Release forks in the same order as acquisition
    if (philo->id % 2 == 0)
    {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
    }
    else
    {
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
    }

    return (1);
}


void    *philosopher_routine(void *arg)
{
    t_philosopher    *philo;

    philo = (t_philosopher *)arg;
    if (!philo)
        return (NULL);

    // Wait for simulation start
    while (true)
    {
        pthread_mutex_lock(philo->time_zero_mut);
        if (philo->time_zero != 0)
        {
            pthread_mutex_unlock(philo->time_zero_mut);
            break;
        }
        pthread_mutex_unlock(philo->time_zero_mut);
        usleep(50);  // Reduced wait time
    }

    // More efficient staggering for large groups
    if (philo->num_philosophers > 100)
    {
        // Divide philosophers into smaller groups
        int group = philo->id % 4;
        usleep(50 * group);  // Minimal stagger between groups
    }
    else if (philo->id % 2 == 0)
        usleep(50);

    while (!check_simulation_stop(philo))
    {
        if (!try_to_eat(philo))
        {
            if (check_death_time(philo))
                continue;  // Try again immediately if close to death
            usleep(100);
        }
        else
        {
            if (philo->is_full)
                break;
            print_status(philo, "is sleeping");
            ft_usleep(philo->time_to_sleep * 1000);
            if (!check_simulation_stop(philo))
                print_status(philo, "is thinking");
        }
    }
    return (NULL);
}
