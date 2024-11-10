/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corin <corin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 08:56:41 by corin             #+#    #+#             */
/*   Updated: 2024/11/10 17:17:49 by corin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

/*
*Assigns forks to each philosopher
*Even numbered philosophers take the right fork first
*Odd numbered philosophers take the left fork first
*/
static void	assign_forks(t_philo *philo)
{
	philo->forks[0] = philo->id;
	philo->forks[1] = (philo->id + 1) % philo->data->num_philos;
	if (philo->id % 2)
	{
		philo->forks[0] = (philo->id + 1) % philo->data->num_philos;
		philo->forks[1] = philo->id;
	}
}

static void	free_philos_array(t_philo **philos, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(philos[i]);
		i++;
	}
	free(philos);
}

static void	init_philo_data(t_philo *philo, int id, t_data *data)
{
	philo->id = id;
	philo->data = data;
	philo->meals_eaten = 0;
	philo->last_meal_time = 0;
	assign_forks(philo);
}

static t_philo	*create_philosopher(int id, t_data *data)
{
	t_philo	*philo;

	philo = ft_calloc(1, sizeof(t_philo));
	if (!philo)
		return (NULL);
	init_philo_data(philo, id, data);
	if (pthread_mutex_init(&philo->last_meal_time_mut, NULL) != 0)
		return (free(philo), NULL);
	return (philo);
}

t_philo	**init_philos(t_data *data)
{
	t_philo	**philos;
	int		i;

	philos = ft_calloc(data->num_philos, sizeof(t_philo *));
	if (!philos)
		return (NULL);
	i = 0;
	while (i < data->num_philos)
	{
		philos[i] = create_philosopher(i, data);
		if (!philos[i])
		{
			free_philos_array(philos, i);
			display_message("Error: Malloc failed\n", 0);
			return (NULL);
		}
		i++;
	}
	return (philos);
}
