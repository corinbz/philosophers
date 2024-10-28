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
	int	status;

	i = 0;
	status = 0;
	while (i < philos_num)
	{
		if (pthread_join(sim->philosophers[i].thread, NULL) != 0)
			status = 1;
		i++;
	}
	if (status)
		return (ft_error("Failed to join threads\n"));
	return (0);
}

void	free_sim_memory(t_simulation *sim)
{
	if (!sim)
		return ;
	if (sim->philosophers)
		free(sim->philosophers);
	if (sim->forks)
		free(sim->forks);
	if (sim->forks_available)
		free(sim->forks_available);
	free(sim);
}

void	cleanup_simulation(t_simulation *sim)
{
	int	i;

	if (!sim)
		return ;
	pthread_mutex_lock(sim->philosophers[0].sim_stop_mut);
	sim->simulation_stop = true;
	pthread_mutex_unlock(sim->philosophers[0].sim_stop_mut);
	i = 0;
	while (i < sim->num_philosophers)
	{
		pthread_join(sim->philosophers[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < sim->num_philosophers)
	{
		pthread_mutex_destroy(&sim->forks[i]);
		if (sim->philosophers[i].time_zero_mut)
		{
			pthread_mutex_destroy(sim->philosophers[i].time_zero_mut);
			free(sim->philosophers[i].time_zero_mut);
		}
		if (sim->philosophers[i].last_meal_mut)
		{
			pthread_mutex_destroy(sim->philosophers[i].last_meal_mut);
			free(sim->philosophers[i].last_meal_mut);
		}
		if (sim->philosophers[i].sim_stop_mut)
		{
			pthread_mutex_destroy(sim->philosophers[i].sim_stop_mut);
			free(sim->philosophers[i].sim_stop_mut);
		}
		i++;
	}
	pthread_mutex_destroy(&sim->print_mutex);
	free_sim_memory(sim);
}
