/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 14:28:30 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 10:46:27 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// set all simulation parameters
int	parse_arguments(t_simulation *sim, int ac, char **av)
{
	sim->num_philosophers = ft_atoi(av[1]);
	sim->time_to_die = ft_atoi(av[2]);
	sim->time_to_eat = ft_atoi(av[3]);
	sim->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		sim->num_times_to_eat = ft_atoi(av[5]);
	else
		sim->num_times_to_eat = -1;
	sim->simulation_stop = false;
	if (sim->num_philosophers <= 0 || sim->num_philosophers > 200
		|| sim->time_to_die <= 0 || sim->time_to_eat <= 0
		|| sim->time_to_sleep <= 0 || (ac == 6 && sim->num_times_to_eat <= 0))
		return (0);
	return (1);
}

// check if all values are digits
bool	valid_args(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (!ft_isdigit(av[i][j]))
				return (false);
			j++;
		}
		i++;
	}
	return (true);
}
