/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 13:38:08 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/03 14:13:01 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	init_program(t_program *program, t_philo *philos)
{
	program->dead_flag = 0;
	program->philos = philos;
	pthread_mutex_init(&program->write_lock, NULL);
	pthread_mutex_init(&program->dead_lock, NULL);
	pthread_mutex_init(&program->meal_lock, NULL);
	return(0);
}

int init_forks(pthread_mutex_t *forks, int total_phil)
{
	int	i;
	
	i = 0;
	while (i < total_phil)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	return (0);
}

int	init_philos(t_args *args, t_philo *philos, t_program *program, pthread_mutex_t *forks)
{
	int	i;
	
	i = 0;
	while (args->total_phil)
	{
		philos[i] = (t_philo){0};
		philos[i].id = i + 1;
		philos[i].start_time = get_current_time();
		philos[i].last_meal = get_current_time();
		philos[i].write_lock = &program->write_lock;
		philos[i].dead_lock = &program->dead_lock;
		philos[i].meal_lock = &program->meal_lock;
		philos[i].dead = &program->dead_flag;
		philos[i].l_fork = &forks[i];
		if (i == 0)
			philos[i].r_fork = &forks[args->total_phil - 1];
		else
			philos[i].r_fork = &forks[i - 1];
		i++;
	}
	return(0);
}
