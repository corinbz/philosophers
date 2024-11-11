/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 08:22:11 by corin             #+#    #+#             */
/*   Updated: 2024/11/11 12:38:58 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

size_t	ft_strlen(const char *str)
{
	const char	*s;

	s = str;
	while (*s)
		s++;
	return (s - str);
}

int	display_message(char *str, int ret)
{
	write(2, str, ft_strlen(str));
	return (ret);
}

static void	print_status(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", get_current_time() - philo->data->time_zero,
		philo->id + 1, str);
}

void	write_status(t_philo *philo, bool monitor_report, char *str)
{
	if (sim_must_stop(philo->data) && monitor_report == false)
		return ;
	pthread_mutex_lock(&philo->data->print_mut);
	print_status(philo, str);
	pthread_mutex_unlock(&philo->data->print_mut);
}
