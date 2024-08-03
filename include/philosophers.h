/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:14:11 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/03 16:15:58 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				meals_eaten;
	long long		last_meal_time;
	int				*simulation_stop;
	pthread_mutex_t	*print_mutex;
	int				time_to_die;
	int				time_to_eat; 
	int				time_to_sleep;
	int				num_times_to_eat;
}					t_philosopher;

typedef struct
{
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat; 
	int				time_to_sleep;
	int				num_times_to_eat;
	int				simulation_stop;
	pthread_mutex_t	print_mutex;
}	t_simulation;

//utils
int		ft_atoi(const char *str);
int		ft_isdigit(int s);
void	*ft_calloc(size_t num_elements, size_t element_size);
size_t	ft_strlen(const char *str);
int	ft_error(const char* err);

//args_utils
bool	valid_args(char **av);

//philo
void *philosopher_routine(void *arg);
void print_status(t_philosopher *philo, const char *status);
//simulation
void start_simulation(t_simulation *sim);
void *monitor_simulation(void *arg);
// //time utils
void cleanup_simulation(t_simulation *sim);
void ft_usleep(long long time);
long long get_current_time(void);

// //init structs
t_simulation	*init_simulation (int ac, char **av);