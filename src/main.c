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

int main(int ac, char **av)
{
	t_args			args;
	t_program		program;
	pthread_mutex_t	forks[200];
	t_philo			philos[200];

	args = (t_args){0};
	if(!get_args(ac, av, &args))
		return (1);
	if(display_args(&args) == 1)
		return (1);
	init_program(&program, philos);
	init_forks(forks, args.total_phil);
	init_philos(&args, philos, &program, forks);
	return (0);
}
