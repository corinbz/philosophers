/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:02:26 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/25 14:00:29 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Program entry point
** @param ac: Argument count
** @param av: Argument values
** @return: 0 on success, non-zero on error
*/
int	main(int ac, char **av)
{
	t_simulation	*sim;
	pthread_t		monitor_thread;
	int				ret;

	sim = init_simulation(ac, av);
	if (!sim)
		return (ft_error("Failed to initialize simulation\n"));
	ret = pthread_create(&monitor_thread, NULL, monitor_simulation, sim);
	if (ret != 0)
	{
		cleanup_simulation(sim);
		return (ft_error("Failed to create monitor thread\n"));
	}
	ret = pthread_join(monitor_thread, NULL);
	if (ret != 0)
		ft_error("Failed to join monitor thread\n");
	cleanup_simulation(sim);
	return (0);
}
