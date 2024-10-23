/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:27:31 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 10:22:17 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_error(const char *err)
{
	write(2, err, ft_strlen(err));
	return (1);
}

void	free_sim_memory(t_simulation *sim)
{
	free(sim->philosophers);
	free(sim->forks);
	free(sim->forks_available);
	free(sim);
}

/*
Waits for all philosopher threads to finish, cleans the mutexes
 and frees the allocated memory
*/
void	cleanup_simulation(t_simulation *sim)
{
	int	i;

	if (!sim)
		return ;
	i = 0;
	while (i < sim->num_philosophers)
	{
		if (pthread_join(sim->philosophers[i].thread, NULL) != 0)
			ft_error("Failed to join thread\n");
		i++;
	}
	i = 0;
	while (i < sim->num_philosophers)
	{
		if (pthread_mutex_destroy(&sim->forks[i]) != 0)
			ft_error("Failed to destroy fork mutex\n");
		i++;
	}
	if (pthread_mutex_destroy(&sim->print_mutex) != 0)
		ft_error("Failed to destroy print mutex\n");
	free(sim->philosophers);
	free(sim->forks);
	free(sim->forks_available);
	free(sim);
}
