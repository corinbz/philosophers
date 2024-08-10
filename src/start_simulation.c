/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:14:50 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/10 15:04:59 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static int all_philosophers_full(t_simulation *sim)
{
    int i;

    for (i = 0; i < sim->num_philosophers; i++)
    {
        if (!sim->philosophers[i].is_full)
            return 0;
    }
    return 1;
}

int start_simulation(t_simulation *sim)
{
    int i;

    for (i = 0; i < sim->num_philosophers; i++) {
        if (pthread_create(&sim->philosophers[i].thread, NULL, philosopher_routine, &sim->philosophers[i]) != 0) {
            sim->simulation_stop = 1;
            return (ft_error("Failed to create philosopher thread\n"));
        }
        ft_usleep(100);
    }
    return 0;
}

void *monitor_simulation(void *arg)
{
    t_simulation *sim;
    int i;
    long long current_time;

    sim = (t_simulation *)arg;
    while (!sim->simulation_stop)
    {
        for (i = 0; i < sim->num_philosophers; i++)
        {
            current_time = get_current_time();
            if (current_time - sim->philosophers[i].last_meal_time > sim->time_to_die)
            {
                print_status(&sim->philosophers[i], "died");
                sim->simulation_stop = 1;
                return NULL;
            }
            if (sim->num_times_to_eat != -1 && 
                sim->philosophers[i].meals_eaten >= sim->num_times_to_eat)
            {
                sim->philosophers[i].is_full = 1;
                if (all_philosophers_full(sim))
                {
                    sim->simulation_stop = 1;
                    return NULL;
                }
            }
        }
        ft_usleep(1000);  // Sleep for 1ms to avoid busy waiting
    }
    return NULL;
}