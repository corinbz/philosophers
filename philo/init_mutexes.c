/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutexes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:18:46 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 15:22:01 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	init_philosopher_mutexes(t_philosopher *philosopher)
{
	philosopher->time_zero_mut = ft_calloc(1, sizeof(pthread_mutex_t));
	philosopher->last_meal_mut = ft_calloc(1, sizeof(pthread_mutex_t));
	philosopher->sim_stop_mut = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!philosopher->time_zero_mut || !philosopher->last_meal_mut
		|| !philosopher->sim_stop_mut)
		return (1);
	if (pthread_mutex_init(philosopher->time_zero_mut, NULL) != 0)
	{
		ft_error("Failed to init time_zero mutex\n");
		return (1);
	}
	if (pthread_mutex_init(philosopher->last_meal_mut, NULL) != 0)
	{
		ft_error("Failed to init last_meal mutex\n");
		return (1);
	}
	if (pthread_mutex_init(philosopher->sim_stop_mut, NULL) != 0)
	{
		ft_error("Failed to init sim_stop mutex\n");
		return (1);
	}
	return (0);
}

static int	destroy_philosopher_mutexes(t_philosopher *philosopher)
{
	int	status;

	status = 0;
	if (pthread_mutex_destroy(philosopher->time_zero_mut) != 0)
	{
		ft_error("Failed to destroy time_zero mutex\n");
		status = 1;
	}
	if (pthread_mutex_destroy(philosopher->last_meal_mut) != 0)
	{
		ft_error("Failed to destroy last_meal mutex\n");
		status = 1;
	}
	if (pthread_mutex_destroy(philosopher->sim_stop_mut) != 0)
	{
		ft_error("Failed to destroy sim_stop mutex\n");
		status = 1;
	}
	return (status);
}

static int	destroy_mutexes_up_to(t_simulation *sim, int index)
{
	int	status;
	int	i;

	status = 0;
	if (pthread_mutex_destroy(&sim->print_mutex) != 0)
	{
		ft_error("Failed to destroy print mutex\n");
		status = 1;
	}
	i = index;
	while (i >= 0)
	{
		if (pthread_mutex_destroy(&sim->forks[i]) != 0)
		{
			ft_error("Failed to destroy fork mutex\n");
			status = 1;
		}
		if (destroy_philosopher_mutexes(&sim->philosophers[i]) != 0)
			status = 1;
		i--;
	}
	return (status);
}

int	init_mutexes(t_simulation *sim)
{
	int	i;

	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (free_sim_memory(sim), 1);
	i = 0;
	while (i < sim->num_philosophers)
	{
		if (init_philosopher_mutexes(&sim->philosophers[i]) != 0)
		{
			destroy_mutexes_up_to(sim, i - 1);
			free_sim_memory(sim);
			return (2);
		}
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
		{
			ft_error("Failed to init fork mutex\n");
			destroy_mutexes_up_to(sim, i);
			free_sim_memory(sim);
			return (2);
		}
		i++;
	}
	return (0);
}
