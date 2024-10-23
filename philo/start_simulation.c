/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:47:27 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 16:12:32 by ccraciun         ###   ########.fr       */
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

void	set_starting_time(t_simulation *sim)
{
	int		i;
	long	start_time;

	i = 0;
	start_time = get_current_time();
	while (i < sim->num_philosophers)
	{
		pthread_mutex_lock(sim->philosophers[i].time_zero_mut);
		pthread_mutex_lock(sim->philosophers[i].last_meal_mut);
		sim->philosophers[i].time_zero = start_time;
		sim->philosophers[i].last_meal_time = start_time;
		pthread_mutex_unlock(sim->philosophers[i].time_zero_mut);
		pthread_mutex_unlock(sim->philosophers[i].last_meal_mut);
		i++;
	}
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
			sim->simulation_stop = true;
			err = 1;
			break ;
		}
		i++;
	}
	if (err)
		return (clean_philo_threads(sim, i));
	set_starting_time(sim);
	return (0);
}

// Checks if philosopher time_to_die > last time he ate
static bool	check_philosopher_health(t_simulation *sim, int i)
{
	long long	current_time;
	long long	ate_last;

	pthread_mutex_lock(sim->philosophers[i].last_meal_mut);
	current_time = get_current_time();
	ate_last = current_time - sim->philosophers[i].last_meal_time;
	pthread_mutex_unlock(sim->philosophers[i].last_meal_mut);
	if (ate_last > sim->time_to_die)
	{
		pthread_mutex_lock(&sim->print_mutex);
		pthread_mutex_lock(sim->philosophers[i].time_zero_mut);
		printf("%lld %d died\n", current_time - sim->philosophers[i].time_zero,
			sim->philosophers[i].id);
		pthread_mutex_lock(sim->philosophers[i].sim_stop_mut);
		*sim->philosophers[i].simulation_stop = true;
		pthread_mutex_unlock(sim->philosophers[i].sim_stop_mut);
		pthread_mutex_unlock(sim->philosophers[i].time_zero_mut);
		pthread_mutex_unlock(&sim->print_mutex);
		return (false);
	}
	return (true);
}

void *monitor_simulation(void *arg)
{
    t_simulation *sim = (t_simulation *)arg;
    int i;
    bool should_continue;

    if (!sim || !sim->philosophers)
        return (NULL);
    while (1)
    {
        i = 0;
        while (i < sim->num_philosophers)
        {
            pthread_mutex_lock(sim->philosophers[i].sim_stop_mut);
            should_continue = !(*sim->philosophers[i].simulation_stop);
            pthread_mutex_unlock(sim->philosophers[i].sim_stop_mut);
            
            if (!should_continue)
                return (NULL);
                
            if (all_philosophers_full(sim))
            {
                pthread_mutex_lock(sim->philosophers[0].sim_stop_mut);
                *sim->philosophers[0].simulation_stop = true;
                pthread_mutex_unlock(sim->philosophers[0].sim_stop_mut);
                pthread_mutex_lock(&sim->print_mutex);
                printf("All philosophers have eaten enough times\n");
                pthread_mutex_unlock(&sim->print_mutex);
                return (NULL);
            }
            
            if (!check_philosopher_health(sim, i))
            {
                pthread_mutex_lock(sim->philosophers[0].sim_stop_mut);
                *sim->philosophers[0].simulation_stop = true;
                pthread_mutex_unlock(sim->philosophers[0].sim_stop_mut);
                pthread_mutex_lock(&sim->print_mutex);
                printf("%lld %d died\n", 
                    get_current_time() - sim->philosophers[i].time_zero,
                    sim->philosophers[i].id);
                pthread_mutex_unlock(&sim->print_mutex);
                return (NULL);
            }
            i++;
        }
        usleep(1000); // Small delay to prevent CPU hogging
    }
    return (NULL);
}