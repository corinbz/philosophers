/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:27:31 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/03 16:29:39 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	ft_error(const char* err)
{
	write(2, err, ft_strlen(err));
	return(1);
}

void cleanup_simulation(t_simulation *sim)
{
    int i;

    // Join all philosopher threads
    for (i = 0; i < sim->num_philosophers; i++)
    {
		// printf("joining\n");
        if(pthread_join(sim->philosophers[i].thread, NULL) != 0)
			ft_error("failed to join thread\n");
    }

	// printf("got herer\n");
    // Destroy all mutexes
    for (i = 0; i < sim->num_philosophers; i++)
    {
        pthread_mutex_destroy(&sim->forks[i]);
    }
    pthread_mutex_destroy(&sim->print_mutex);

    // Free allocated memory
    free(sim->philosophers);
    free(sim->forks);
    free(sim);
}