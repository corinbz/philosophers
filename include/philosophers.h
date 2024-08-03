/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:14:11 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/03 14:12:35 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_args
{
	int	total_phil;
	int	forks;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	max_meals;
}	t_args;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				eating;
	int				meals_eaten;
	size_t			last_meal;
	size_t			start_time;
	int				*dead;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*meal_lock;
}					t_philo;

typedef struct s_program
{
	int				dead_flag;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	write_lock;
	t_philo			*philos;
}					t_program;

//utils
int		ft_atoi(const char *str);
int		ft_isdigit(int s);

//args_utils
bool	get_args (int ac, char **av, t_args *args);
int		display_args(t_args *args);
bool	valid_args(char **av);

//time utils
int		ft_usleep(size_t milliseconds);
size_t	get_current_time(void);

//init structs
int		init_program(t_program *program, t_philo *philos);
int		init_forks(pthread_mutex_t *forks, int total_phil);
int		init_philos(t_args *args, t_philo *philos, t_program *program, pthread_mutex_t *forks);