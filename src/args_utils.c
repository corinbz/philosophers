/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:59:01 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/03 13:11:11 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

bool get_args (int ac, char **av, t_args *args)
{
	if (ac == 5 || ac == 6)
	{
		if(!valid_args(av))
			return(printf("Arguments must be digits only\n"), false);
		args->total_phil = ft_atoi(av[1]);
		args->forks = args->total_phil;
		args->time_to_die = ft_atoi(av[2]);
		args->time_to_eat = ft_atoi(av[3]);
		args->time_to_sleep = ft_atoi(av[4]);
		if (ac == 6)
		args->max_meals = ft_atoi(av[5]);
		return (true);
	}
	printf("Invalid number of arguments\n");
	return (false);
}

int	display_args(t_args *args)
{
	printf("Total philosophers: %d\n", args->total_phil);
	printf("Forks: %d\n", args->forks);
	printf("Time to die: %d\n", args->time_to_die);
	printf("Time to eat: %d\n", args->time_to_eat);
	printf("Time to sleep: %d\n", args->time_to_sleep);
	printf("Max meals: %d\n", args->max_meals);
	if(args->total_phil > 200)
		return(printf("Error: Maximum number of philosophers is 200\n"), 1);
	return (0);
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