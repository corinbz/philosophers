/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:40:46 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/21 09:46:44 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static	int	allocate_resources(t_simulation	*sim)
{
	int	i;

	i = 0;
	sim->philosophers = malloc(sizeof(t_philosopher) * sim->num_philosophers);
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philosophers);
	if (!sim->philosophers || !sim->forks)
	{
		free(sim->philosophers);
		free(sim->forks);
		return (0);
	}
	return (1);
}

static	int	init_mutexes(t_simulation	*sim)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (0);
	while (i < sim->num_philosophers)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

static	void	init_philosophers(t_simulation	*sim)
{
	int			i;
	long long	start_time;

	start_time = get_current_time();
	i = 0;
	while (i < sim->num_philosophers)
	{
		sim->philosophers[i].id = i + 1;
		sim->philosophers[i].left_fork = &sim->forks[i];
		sim->philosophers[i].right_fork = &sim->forks[(i + 1)
			%sim->num_philosophers];
		sim->philosophers[i].meals_eaten = 0;
		sim->philosophers[i].last_meal_time = start_time;
		sim->philosophers[i].simulation_stop = &sim->simulation_stop;
		sim->philosophers[i].print_mutex = &sim->print_mutex;
		sim->philosophers[i].time_to_die = sim->time_to_die;
		sim->philosophers[i].time_to_eat = sim->time_to_eat;
		sim->philosophers[i].time_to_sleep = sim->time_to_sleep;
		sim->philosophers[i].num_times_to_eat = sim->num_times_to_eat;
		sim->philosophers[i].num_philosophers = sim->num_philosophers;
		sim->philosophers[i].is_full = false;
		i++;
	}
}

t_simulation	*init_simulation(int ac, char **av)
{
	t_simulation	*sim;

	if (ac != 5 && ac != 6)
		return (ft_error("Invalid number of arguments\n"), NULL);
	if (!valid_args(av))
		return (ft_error("Arguments must be positive digits only\n"), NULL);
	sim = ft_calloc(1, sizeof(t_simulation));
	if (!sim)
		return (ft_error("Memory allocation failed\n"), NULL);
	if (!parse_arguments(sim, ac, av))
		return (ft_error("Invalid argument values\n"), NULL);
	if (!allocate_resources(sim))
		return ((free(sim)), ft_error("Failed to allocate resources\n"), NULL);
	if (!init_mutexes(sim))
	{
		cleanup_simulation(sim);
		return (ft_error("Failed to initialize mutexes\n"), NULL);
	}
	sim->simulation_stop = false;
	init_philosophers(sim);
	return (sim);
}
