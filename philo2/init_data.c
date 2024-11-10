/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corin <corin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 08:56:41 by corin             #+#    #+#             */
/*   Updated: 2024/11/10 17:15:39 by corin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

static pthread_mutex_t	*init_forks_mutexes(t_data *data)
{
	pthread_mutex_t	*forks_mut;
	int				i;

	i = 0;
	forks_mut = ft_calloc(data->num_philos, sizeof(pthread_mutex_t));
	if (!forks_mut)
		return (display_message("Error: Malloc failed\n", 0), NULL);
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&forks_mut[i], NULL) != 0)
		{
			display_message("Error: Mutex init failed\n", 0);
			while (i-- >= 0)
			{
				pthread_mutex_destroy(&forks_mut[i]);
				free(&forks_mut[i]);
			}
			free(forks_mut);
			return (NULL);
		}
		i++;
	}
	return (forks_mut);
}

void	free_forks_mut(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks_mut[i]);
		i++;
	}
	free(data->forks_mut);
}

static bool	init_data_mutexes(t_data *data)
{
	data->forks_mut = init_forks_mutexes(data);
	if (!data->forks_mut)
		return (false);
	if (pthread_mutex_init(&data->print_mut, NULL) != 0
		|| pthread_mutex_init(&data->stop_sim_mut, NULL) != 0)
	{
		display_message("Error: Mutex init failed\n", 0);
		free_forks_mut(data);
		return (false);
	}
	return (true);
}

t_data	*init_data(int ac, char **av)
{
	t_data	*data;

	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (NULL);
	if (!all_digits(av))
		return (free(data), NULL);
	if (!parse_arguments(data, ac, av))
		return (free(data), NULL);
	data->philos = init_philos(data);
	if (!data->philos)
		return (free(data), NULL);
	if (!init_data_mutexes(data))
		return (free(data), NULL);
	data->stop_simulation = false;
	return (data);
}
