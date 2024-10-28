/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:47:27 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/25 14:11:20 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int    all_philosophers_full(t_simulation *sim)
{
    int i;
    bool all_full;

    all_full = true;
    for (i = 0; i < sim->num_philosophers; i++)
    {
        pthread_mutex_lock(sim->philosophers[i].last_meal_mut);
        if (!sim->philosophers[i].is_full)
        {
            all_full = false;
            pthread_mutex_unlock(sim->philosophers[i].last_meal_mut);
            break;
        }
        pthread_mutex_unlock(sim->philosophers[i].last_meal_mut);
    }

    if (all_full)
    {
        pthread_mutex_lock(sim->philosophers[0].sim_stop_mut);
        if (!sim->simulation_stop)
        {
            pthread_mutex_lock(&sim->print_mutex);
            printf("All philosophers have eaten enough times\n");
            sim->simulation_stop = true;
            pthread_mutex_unlock(&sim->print_mutex);
        }
        pthread_mutex_unlock(sim->philosophers[0].sim_stop_mut);
    }

    return (all_full);
}

void    set_starting_time(t_simulation *sim)
{
    int        i;
    long    start_time;

    start_time = get_current_time();

    // Set all start times atomically
    for (i = 0; i < sim->num_philosophers; i++)
    {
        pthread_mutex_lock(sim->philosophers[i].time_zero_mut);
        pthread_mutex_lock(sim->philosophers[i].last_meal_mut);
        sim->philosophers[i].time_zero = start_time;
        sim->philosophers[i].last_meal_time = start_time;
        pthread_mutex_unlock(sim->philosophers[i].last_meal_mut);
        pthread_mutex_unlock(sim->philosophers[i].time_zero_mut);
    }
}

static bool    check_philosopher_health(t_simulation *sim, int i)
{
    long long    current_time;
    long long    ate_last;

    pthread_mutex_lock(sim->philosophers[i].last_meal_mut);
    if (!sim->philosophers[i].is_full)
    {
        current_time = get_current_time();
        ate_last = current_time - sim->philosophers[i].last_meal_time;

        if (ate_last > sim->time_to_die)
        {
            pthread_mutex_lock(&sim->print_mutex);
            printf("%lld %d died\n",
                current_time - sim->philosophers[i].time_zero,
                sim->philosophers[i].id);
            pthread_mutex_lock(sim->philosophers[i].sim_stop_mut);
            sim->simulation_stop = true;
            pthread_mutex_unlock(sim->philosophers[i].sim_stop_mut);
            pthread_mutex_unlock(&sim->print_mutex);
            pthread_mutex_unlock(sim->philosophers[i].last_meal_mut);
            return (false);
        }
    }
    pthread_mutex_unlock(sim->philosophers[i].last_meal_mut);
    return (true);
}

int    start_simulation(t_simulation *sim)
{
    int    i;
    int    err;

    i = 0;
    err = 0;

    // Create all threads quickly
    while (i < sim->num_philosophers)
    {
        if (pthread_create(&sim->philosophers[i].thread,
                NULL, philosopher_routine, &sim->philosophers[i]) != 0)
        {
            sim->simulation_stop = true;
            err = 1;
            break;
        }
        i++;
        if (sim->num_philosophers > 100)
            usleep(50);  // Small delay between thread creation for large numbers
    }

    if (err)
        return (clean_philo_threads(sim, i));

    // Set start time after all threads are created
    set_starting_time(sim);
    return (0);
}

void    *monitor_simulation(void *arg)
{
    t_simulation    *sim;
    int            i;
    bool        should_continue;

    sim = (t_simulation *)arg;
    if (!sim || !sim->philosophers)
        return (NULL);

    ft_usleep(100 * 1000);

    while (1)
    {
        pthread_mutex_lock(sim->philosophers[0].sim_stop_mut);
        should_continue = !sim->simulation_stop;
        pthread_mutex_unlock(sim->philosophers[0].sim_stop_mut);

        if (!should_continue)
            return (NULL);

        // Check philosophers' health
        i = 0;
        while (i < sim->num_philosophers)
        {
            if (!check_philosopher_health(sim, i))
                return (NULL);
            i++;
        }

        // Check if all philosophers are full
        if (all_philosophers_full(sim))
            return (NULL);

        ft_usleep(1000);
    }
    return (NULL);
}
