/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 08:48:41 by corin             #+#    #+#             */
/*   Updated: 2024/11/11 09:35:00 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	main(int ac, char **av)
{
	t_data	*data;

	if (ac != 5 && ac != 6)
		return (display_message("Error: Wrong number of arguments\n", 1));
	data = NULL;
	data = init_data(ac, av);
	if (!data)
		return (display_message("Error: Data init fail\n", 1), EXIT_FAILURE);
	if (!start_sim(data))
	{
		return (clean_all(data), EXIT_FAILURE);
	}
	clean_all(data);
	return (EXIT_SUCCESS);
}
