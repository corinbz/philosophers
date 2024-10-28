/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:40:46 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/25 14:10:15 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	allocate_resources(t_simulation *sim)
{
	sim->philosophers = malloc(sizeof(t_philosopher) * sim->num_philosophers);
	if (!sim->philosophers)
		return (0);

	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philosophers);
	if (!sim->forks)
	{
		free(sim->philosophers);
		return (0);
	}
	return (1);
}

void	set_fork_ids(t_simulation *sim, int i)
{
	if (sim->num_philosophers == 1)
	{
		sim->philosophers[i].left_fork_id = 0;
		sim->philosophers[i].right_fork_id = 0;
		return ;
	}
	if (sim->philosophers[i].id % 2 != 0)
	{
		sim->philosophers[i].left_fork_id = (sim->philosophers[i].id - 1);
		sim->philosophers[i].right_fork_id = sim->philosophers[i].id
			% sim->num_philosophers;
	}
	else
	{
		sim->philosophers[i].left_fork_id = sim->philosophers[i].id
			% sim->num_philosophers;
		sim->philosophers[i].right_fork_id = sim->philosophers[i].id - 1;
	}
}

void	init_philosophers(t_simulation *sim)
{
	int		i;
	long	start_time;

	start_time = get_current_time();
	i = 0;
	while (i < sim->num_philosophers)
	{
		sim->philosophers[i].id = i + 1;
		sim->philosophers[i].left_fork = &sim->forks[i];
		sim->philosophers[i].right_fork = &sim->forks[(i + 1)
			% sim->num_philosophers];
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
		sim->philosophers[i].time_zero = 0;
		set_fork_ids(sim, i);
		i++;
	}
}

t_simulation	*init_simulation(int ac, char **av)
{
	t_simulation	*sim;
	int				init_mutex;

	if (ac != 5 && ac != 6)
		return (ft_error("Invalid number of arguments\n"), NULL);
	if (!valid_args(av))
		return (ft_error("Arguments must be positive digits only\n"), NULL);
	sim = ft_calloc(1, sizeof(t_simulation));
	if (!sim)
		return (ft_error("Memory allocation failed\n"), NULL);
	if (!parse_arguments(sim, ac, av))
		return ((free(sim)), ft_error("Invalid argument values\n"), NULL);
	if (!allocate_resources(sim))
		return ((free(sim)), ft_error("Failed to allocate resources\n"), NULL);
	init_mutex = init_mutexes(sim);
	if (init_mutex > 0)
	{
		if (init_mutex == 2)
			return (ft_error("Failed to initialize mutexes\n"), NULL);
		return (ft_error("Failed to initialize mutexes\n"), NULL);
	}
	sim->simulation_stop = false;
	init_philosophers(sim);
	if (start_simulation(sim) != 0)
	{
		cleanup_simulation(sim);
		return (ft_error("Failed to start simulation\n"), NULL);
	}
	return (sim);
}
