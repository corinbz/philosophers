/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:27:31 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 15:10:39 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_error(const char *err)
{
	write(2, err, ft_strlen(err));
	return (1);
}

int	clean_philo_threads(t_simulation *sim, int philos_num)
{
	int	i;

	i = 0;
	while (i < philos_num)
	{
		pthread_join(sim->philosophers[philos_num].thread, NULL);
		i++;
	}
	return (ft_error("Failed to join create thread\n"));
}

void	free_sim_memory(t_simulation *sim)
{
	free(sim->philosophers);
	free(sim->forks);
	free(sim->forks_available);
	free(sim);
}

void	join_philo_thread(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philosophers)
	{
		if (pthread_join(sim->philosophers[i].thread, NULL) != 0)
			ft_error("Failed to join thread\n");
		i++;
	}
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
	join_philo_thread(sim);
	while (i < sim->num_philosophers)
	{
		if (pthread_mutex_destroy(&sim->forks[i]) != 0)
			ft_error("Failed to destroy fork mutex\n");
		if (pthread_mutex_destroy(sim->philosophers[i].time_zero_mut) != 0)
			ft_error("Failed to destroy time_zero mutex\n");
		if (pthread_mutex_destroy(sim->philosophers[i].last_meal_mut) != 0)
			ft_error("Failed to destroy last_meal mutex\n");
		free(sim->philosophers[i].last_meal_mut);
		free(sim->philosophers[i].time_zero_mut);
		free(sim->philosophers[i].sim_stop_mut);
		i++;
	}
	if (pthread_mutex_destroy(&sim->print_mutex) != 0)
		ft_error("Failed to destroy print mutex\n");
	free_sim_memory(sim);
}
