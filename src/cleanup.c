/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:27:31 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/10 14:40:04 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int ft_error(const char *err)
{
    write(2, err, ft_strlen(err));
    return (1);
}

void cleanup_simulation(t_simulation *sim)
{
    int i;

    if (!sim)
        return;

    // Join all philosopher threads
    for (i = 0; i < sim->num_philosophers; i++)
    {
        if (pthread_join(sim->philosophers[i].thread, NULL) != 0)
            ft_error("Failed to join thread\n");
    }

    // Destroy all mutexes
    for (i = 0; i < sim->num_philosophers; i++)
    {
        if (pthread_mutex_destroy(&sim->forks[i]) != 0)
            ft_error("Failed to destroy fork mutex\n");
    }

    if (pthread_mutex_destroy(&sim->print_mutex) != 0)
        ft_error("Failed to destroy print mutex\n");

    if (pthread_mutex_destroy(&sim->waiter_mutex) != 0)
        ft_error("Failed to destroy waiter mutex\n");

    // Free allocated memory
    free(sim->philosophers);
    free(sim->forks);
    free(sim->forks_available);
    free(sim);
}