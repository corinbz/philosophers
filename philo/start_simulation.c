/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:47:27 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 09:49:01 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static	int	all_philosophers_full(t_simulation	*sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philosophers)
	{
		if (!sim->philosophers[i].is_full)
			return (0);
		i++;
	}
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
	return (ft_error("Failed to create philosopher	thread\n"));
}

//create thread for each philo and runs the routine for each
int	start_simulation(t_simulation	*sim)
{
	int	i;
	int	err;

	i = 0;
	err = 0;
	while (i < sim->num_philosophers)
	{
		if (pthread_create(&sim->philosophers[i].thread,
				NULL, philosopher_routine, &sim->philosophers[i]) != 0)
		{
			sim->simulation_stop = 1;
			err = 1;
			break ;
		}
		i++;
	}
	if (err)
		return (clean_philo_threads(sim, i));
	return (0);
}

// Checks if philosopher time_to_die > last time he ate
static bool	check_philosopher_health(t_simulation *sim, int i)
{
	long long	current_time;
	long long	ate_last;

	current_time = get_current_time();
	ate_last = current_time - sim->philosophers[i].last_meal_time;
	if (ate_last > sim->time_to_die)
	{
		pthread_mutex_lock(&sim->print_mutex);
		printf("%lld %d died\n", current_time, sim->philosophers[i].id);
		sim->simulation_stop = 1;
		pthread_mutex_unlock(&sim->print_mutex);
		return (false);
	}
	return (true);
}

//Main thread that constantly checks if philosophers ate x times or one of them
//died
void	*monitor_simulation(void	*arg)
{
	t_simulation	*sim;
	int				i;

	sim = (t_simulation *)arg;
	i = 0;
	while (!sim->simulation_stop)
	{
		if (all_philosophers_full(sim))
		{
			sim->simulation_stop = 1;
			return (NULL);
		}
		i = 0;
		while (i < sim->num_philosophers)
		{
			if (!check_philosopher_health(sim, i))
				return (NULL);
			i++;
		}
	}
	return (NULL);
}
