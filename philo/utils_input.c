/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 14:28:30 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 10:46:27 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Checks if a character is a whitespace character
** @param c: Character to check
** @return: 1 if whitespace, 0 otherwise
*/
static int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\v'
		|| c == '\n' || c == '\f' || c == '\r');
}

/*
** Validates and parses command line arguments
** @param sim: Pointer to simulation structure
** @param ac: Argument count
** @param av: Argument values
** @return: 1 on success, 0 on failure
*/
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

/*
** Validates that all command line arguments are digits
** @param av: Array of argument strings
** @return: true if all arguments are valid digits, false otherwise
*/
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

/*
** Converts a string to an integer, handling whitespace and signs
** @param str: String to convert
** @return: Converted integer value or INT_MIN on overflow
*/
int	ft_atoi(const char *str)
{
	long int	result;
	int			sign;

	result = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (result * sign > INT_MAX)
			return (INT_MIN);
		if (result * sign < INT_MIN)
			return (INT_MIN);
		str++;
	}
	return ((int)(result * sign));
}

/*
** Checks if a character is a digit
** @param c: Character to check
** @return: 1 if digit, 0 otherwise
*/
int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}
