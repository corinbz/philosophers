/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:14:50 by ccraciun			       #+#    #+#             */
/*   Updated: 2024/08/10 15:49:38 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../include/philosophers.h"

static	int	all_philosophers_full(t_simulation	*sim)
{
				int	i;

				for	(i	=	0;	i	<	sim->num_philosophers;	i++)
				{
								if	(!sim->philosophers[i].is_full)
												return	0;
				}
				return	1;
}

int	start_simulation(t_simulation	*sim)
{
				int	i;

				for	(i	=	0;	i	<	sim->num_philosophers;	i++)	{
								if	(pthread_create(&sim->philosophers[i].thread,	NULL,	philosopher_routine,	&sim->philosophers[i])	!=	0)	{
												sim->simulation_stop	=	1;
												return	(ft_error("Failed	to	create	philosopher	thread\n"));
								}
								ft_usleep(100);
				}
				return	0;
}

void *monitor_simulation(void *arg)
{
    t_simulation *sim = (t_simulation *)arg;
    int i;
    long long current_time;

    while (!sim->simulation_stop)
    {
        for (i = 0; i < sim->num_philosophers; i++)
        {
            current_time = get_current_time();
            long long time_since_last_meal = current_time - sim->philosophers[i].last_meal_time;
            
            printf("Debug: Philosopher %d last ate %lld ms ago (current: %lld, last: %lld)\n", 
                   sim->philosophers[i].id, time_since_last_meal, current_time, sim->philosophers[i].last_meal_time);
            
            if (time_since_last_meal > sim->time_to_die)
            {
                pthread_mutex_lock(&sim->print_mutex);
                printf("%lld %d died\n", current_time, sim->philosophers[i].id);
                sim->simulation_stop = 1;
                pthread_mutex_unlock(&sim->print_mutex);
                return NULL;
            }
        }
        ft_usleep(1000);  // Check every 1ms
    }
    return NULL;
}
