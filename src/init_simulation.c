/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:59:01 by ccraciun          #+#    #+#             */
/*   Updated: 2024/09/08 11:57:21 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static int parse_arguments(t_simulation *sim, int ac, char **av)
{
sim->num_philosophers = ft_atoi(av[1]);
sim->time_to_die = ft_atoi(av[2]);
sim->time_to_eat = ft_atoi(av[3]);
sim->time_to_sleep = ft_atoi(av[4]);
sim->num_times_to_eat = (ac == 6) ? ft_atoi(av[5]) : -1;
sim->simulation_stop = 0;
if (sim->num_philosophers <= 0 || sim->num_philosophers > 200 ||
	sim->time_to_die <= 0 || sim->time_to_eat <= 0 || sim->time_to_sleep <= 0 ||
	(ac == 6 && sim->num_times_to_eat <= 0))
	return 0;
return 1;
}

bool valid_args(char **av)
{
	int i;
	int j;

	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (!ft_isdigit(av[i][j]))
				return false;
			j++;
		}
		i++;
	}
	return true;
}

static int allocate_resources(t_simulation *sim)
{
	sim->philosophers = malloc(sizeof(t_philosopher) * sim->num_philosophers);
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philosophers);
	sim->forks_available = malloc(sizeof(bool) * sim->num_philosophers);
	if (!sim->philosophers || !sim->forks || !sim->forks_available) {
		free(sim->philosophers);
		free(sim->forks);
		free(sim->forks_available);
		return 0;
	}
	for (int i = 0; i < sim->num_philosophers; i++) {
		sim->forks_available[i] = true;
	}
	return 1;
}

static int init_mutexes(t_simulation *sim)
{
	int i;

	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0 ||
		pthread_mutex_init(&sim->waiter_mutex, NULL) != 0)
		return 0;

	for (i = 0; i < sim->num_philosophers; i++)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
			return 0;
	}
	return 1;
}

static void init_philosophers(t_simulation *sim)
{
	int i;
	long long start_time = get_current_time();

	for (i = 0; i < sim->num_philosophers; i++)
	{
		sim->philosophers[i].id = i + 1;
		sim->philosophers[i].left_fork = &sim->forks[i];
		sim->philosophers[i].right_fork = &sim->forks[(i + 1) % sim->num_philosophers];
		sim->philosophers[i].meals_eaten = 0;
		sim->philosophers[i].last_meal_time = start_time;
		sim->philosophers[i].simulation_stop = &sim->simulation_stop;
		sim->philosophers[i].print_mutex = &sim->print_mutex;
		sim->philosophers[i].time_to_die = sim->time_to_die;
		sim->philosophers[i].time_to_eat = sim->time_to_eat;
		sim->philosophers[i].time_to_sleep = sim->time_to_sleep;
		sim->philosophers[i].num_times_to_eat = sim->num_times_to_eat;
		sim->philosophers[i].waiter_mutex = &sim->waiter_mutex;
		sim->philosophers[i].forks_available = sim->forks_available;
		sim->philosophers[i].num_philosophers = sim->num_philosophers;
	}
}


t_simulation *init_simulation(int ac, char **av)
{
	t_simulation *sim;

	if (ac != 5 && ac != 6)
		return (ft_error("Invalid number of arguments\n"), NULL);
	if (!valid_args(av))
		return (ft_error("Arguments must be positive digits only\n"), NULL);
	sim = ft_calloc(1, sizeof(t_simulation));
	if (!sim)
		return (ft_error("Memory allocation failed\n"), NULL);
	if (!parse_arguments(sim, ac, av))
	{
		return (ft_error("Invalid argument values\n"), NULL);
	}
	if (!allocate_resources(sim))
	{
		free(sim);
		return (ft_error("Failed to allocate resources\n"), NULL);
	}

	if (!init_mutexes(sim))
	{
		cleanup_simulation(sim);
		return (ft_error("Failed to initialize mutexes\n"), NULL);
	}

	init_philosophers(sim);
	return sim;
}
