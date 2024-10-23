/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:40:46 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 13:40:01 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
//init mutexes and cleans up accordingly if something fails
static	int	init_mutexes(t_simulation	*sim)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (free_sim_memory(sim), 1);
	while (i < sim->num_philosophers)
	{
		sim->philosophers[i].time_zero_mut = ft_calloc(1, sizeof(pthread_mutex_t));
		sim->philosophers[i].last_meal_mut = ft_calloc(1, sizeof(pthread_mutex_t));
		sim->philosophers[i].sim_stop_mut = ft_calloc(1, sizeof(pthread_mutex_t));
		if(pthread_mutex_init(sim->philosophers[i].time_zero_mut, NULL) != 0)
			ft_error("Failed to init time_zero mutex\n");
		if(pthread_mutex_init(sim->philosophers[i].last_meal_mut, NULL) != 0)
			ft_error("Failed to init last_meal mutex\n");
		if(pthread_mutex_init(sim->philosophers[i].sim_stop_mut, NULL) != 0)
			ft_error("Failed to init sim_stop mutex\n");
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
		{
			if (pthread_mutex_destroy(&sim->print_mutex) != 0)
				ft_error("Failed to destroy print mutex\n");
			while (i >= 0)
			{
				if (pthread_mutex_destroy(&sim->forks[i]) != 0)
					ft_error("Failed to destroy fork mutex\n");
				if (pthread_mutex_destroy(sim->philosophers[i].time_zero_mut) != 0)
					ft_error("Failed to destroy time_zero mutex\n");
				if (pthread_mutex_destroy(sim->philosophers[i].last_meal_mut) != 0)
					ft_error("Failed to destroy last_meal mutex\n");
				if (pthread_mutex_destroy(sim->philosophers[i].sim_stop_mut) != 0)
					ft_error("Failed to destroy sim_stop mutex\n");
				i--;
			}
			free_sim_memory(sim);
			return (2);
		}
		i++;
	}
	return (0);
}
void	set_fork_ids(t_simulation *sim, int i)
{
	if (sim->philosophers[i].id % 2 != 0)
			sim->philosophers[i].left_fork_id = (sim->philosophers[i].id - 1);
	else
		sim->philosophers[i].left_fork_id = sim->philosophers[i].id % sim->philosophers[i].num_philosophers;
	if (sim->philosophers[i].id % 2 != 0)
		sim->philosophers[i].right_fork_id = sim->philosophers[i].id % sim->philosophers[i].num_philosophers;
	else
		sim->philosophers[i].right_fork_id = sim->philosophers[i].id - 1;
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
		sim->philosophers[i].time_zero = 0;
		set_fork_ids(sim, i);
		i++;
	}
}

/*
Make some basic checks of the arguments and create the simulation data structure

*/
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
		// cleanup_simulation(sim);
		if(init_mutex == 2)
			return (ft_error("Failed to initialize mutexes\n"), NULL);
		return (ft_error("Failed to initialize mutexes\n"), NULL);
	}
	sim->simulation_stop = false;
	init_philosophers(sim);
	return (sim);
}
