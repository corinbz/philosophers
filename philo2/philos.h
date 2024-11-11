/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 07:41:52 by corin             #+#    #+#             */
/*   Updated: 2024/11/11 12:03:43 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOS_H
# define PHILOS_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>

/* constants */

/* structures */
typedef struct s_philo			t_philo;
typedef struct s_data			t_data;

struct s_philo
{
	t_data			*data;
	int				meals_eaten;
	long			last_meal_time;
	int				id;
	int				forks[2];
	pthread_mutex_t	last_meal_time_mut;
	pthread_t		philo_thread;
};

struct s_data
{
	t_philo			**philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_philos;
	int				meals_to_full;
	long			time_zero;
	bool			stop_simulation;
	pthread_mutex_t	print_mut;
	pthread_mutex_t	stop_sim_mut;
	pthread_mutex_t	*forks_mut;
	pthread_t		monitor_thread;
};

//init_data.c
t_data	*init_data(int ac, char **av);
void	free_forks_mut(t_data *data);

//time_utils.c
void	ft_usleep(long ms, t_data *data);
long	get_current_time(void);
void	wait_for_start(long time_zero);

//parse_arguments.c
bool	parse_arguments(t_data *data, int ac, char **av);
bool	all_digits(char **av);
int		ft_atoi(const char *str);

//memory_utils.c
void	*ft_calloc(size_t num_elements, size_t element_size);

//philo_routing.c
void	*philo_routine(void *arg);
bool	sim_must_stop(t_data *data);

//init_philos.c
t_philo	**init_philos(t_data *data);

//monitor_routine.c
void	*monitor_routine(void *arg);

//start_sim.c
bool	start_sim(t_data *data);

//print_utils.c
size_t	ft_strlen(const char *str);
int		display_message(char *str, int ret);
void	write_status(t_philo *philo, bool monitor_report, char *str);

//cleanup.c
void	clean_all(t_data *data);

#endif
