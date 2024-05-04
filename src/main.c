/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corin <corin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:02:26 by ccraciun          #+#    #+#             */
/*   Updated: 2024/04/30 21:09:54by corin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void* routine()
{
	printf("Hello from thread\n");
	return (void*) NULL;
}
bool get_args (int ac, char **av, t_args *args)
{
	if (ac == 5 || ac == 6)
	{
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

void display_args(t_args *args)
{
    printf("Total philosophers: %d\n", args->total_phil);
    printf("Forks: %d\n", args->forks);
    printf("Time to die: %d\n", args->time_to_die);
    printf("Time to eat: %d\n", args->time_to_eat);
    printf("Time to sleep: %d\n", args->time_to_sleep);
    printf("Max meals: %d\n", args->max_meals);
}
int main(int ac, char **av)
{
	t_args args;

	args = (t_args){0};
	if(!get_args(ac, av, &args))
		return (1);
	display_args(&args);
	return (0);
}