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

int main(int ac, char **av)
{
	t_simulation *sim;
	pthread_t monitor_thread;

	sim = init_simulation(ac, av);
	if (!sim)
		return(ft_error("Failed to initialize simulation\n"));
	start_simulation(sim);
	pthread_create(&monitor_thread, NULL, monitor_simulation, sim);
	pthread_join(monitor_thread, NULL);
	cleanup_simulation(sim);
	return (0);
}
