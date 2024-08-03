/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:59:01 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/03 16:21:40 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static void parse_arguments(t_simulation *sim, int ac, char **av)
{
	sim->num_philosophers = atoi(av[1]);
	sim->time_to_die = atoi(av[2]);
	sim->time_to_eat = atoi(av[3]);
	sim->time_to_sleep = atoi(av[4]);
	sim->num_times_to_eat = -1;
	if(ac == 6)
		sim->num_times_to_eat = atoi(av[5]);
	sim->simulation_stop = 0;
}
bool	valid_args(char **av)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while(av[i])
	{
		while(av[i][j])
		{
			if(!ft_isdigit(av[i][j]))
				return(false);
			j++;
		}
		j = 0;
		i++;
	}
	return(true);
}
static int allocate_resources(t_simulation *sim) 
{
	sim->philosophers = malloc(sizeof(t_philosopher) * sim->num_philosophers);
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philosophers);
	if (!sim->philosophers || !sim->forks) {
		free(sim->philosophers);
		free(sim->forks);
		return 0;
	}
	return 1;
}

static void init_mutexes(t_simulation *sim)
{
	int	i;

	i = 0;
	pthread_mutex_init(&sim->print_mutex, NULL);
	while (i < sim->num_philosophers)
	{
		pthread_mutex_init(&sim->forks[i], NULL);
		i++;
	}
}

static void init_philosophers(t_simulation *sim)
{
    int i;

    for (i = 0; i < sim->num_philosophers; i++)
    {
        sim->philosophers[i].id = i + 1;
        sim->philosophers[i].left_fork = &sim->forks[i];
        if (sim->num_philosophers > 1)
            sim->philosophers[i].right_fork = &sim->forks[(i + 1) % sim->num_philosophers];
        else
            sim->philosophers[i].right_fork = &sim->forks[i];
        sim->philosophers[i].meals_eaten = 0;
        sim->philosophers[i].last_meal_time = get_current_time();
        sim->philosophers[i].simulation_stop = &sim->simulation_stop;
        sim->philosophers[i].print_mutex = &sim->print_mutex;
        sim->philosophers[i].time_to_die = sim->time_to_die;
        sim->philosophers[i].time_to_eat = sim->time_to_eat;
        sim->philosophers[i].time_to_sleep = sim->time_to_sleep;
        sim->philosophers[i].num_times_to_eat = sim->num_times_to_eat;
    }
}
t_simulation	*init_simulation (int ac, char **av)
{
	t_simulation	*sim;

	if (ac != 5 && ac != 6)
	{
		ft_error("Invalid number of arguments\n");
		return (NULL);
	}
	if(ft_atoi(av[1]) > 200)
		return(ft_error("You can have a maximum of 200 philosophers\n"), NULL);
	if(!valid_args(av))
		return(ft_error("Arguments must be digits only\n"), NULL);
	sim = ft_calloc(1, sizeof(t_simulation));
	if (!sim)
		return (NULL);
	parse_arguments(sim, ac, av);
	if (!allocate_resources(sim))
		return(free(sim), NULL);
	init_mutexes(sim);
	init_philosophers(sim);
	return (sim);
}
