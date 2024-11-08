/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:14:11 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 15:40:16 by ccraciun         #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>

/* Configuration constants */
# define MAX_PHILOSOPHERS 200
# define MIN_TIME 60

/* Forward declarations of main structures */
typedef struct s_philosopher	t_philosopher;
typedef struct s_simulation		t_simulation;

/* Philosopher structure */
struct s_philosopher
{
	pthread_mutex_t	*time_zero_mut;
	pthread_mutex_t	*last_meal_mut;
	pthread_mutex_t	*sim_stop_mut;
	long			time_zero;
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				left_fork_id;
	int				right_fork_id;
	int				meals_eaten;
	long			last_meal_time;
	bool			*simulation_stop;
	pthread_mutex_t	*print_mutex;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_to_eat;
	int				num_philosophers;
	bool			is_full;
};

/* Simulation structure */
struct s_simulation
{
	t_philosopher	*philosophers;
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_to_eat;
	bool			simulation_stop;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	sim_stop_mut;
	pthread_mutex_t	time_zero_mut;
};

/* Core simulation functions */
t_simulation	*init_simulation(int ac, char **av);
int				start_simulation(t_simulation *sim);
void			*monitor_simulation(void *arg);

/* Philosopher initialization and management */
void			init_philosophers(t_simulation *sim);
void			set_fork_ids(t_simulation *sim, int i);
int				init_mutexes(t_simulation *sim);
void			set_starting_time(t_simulation *sim);

/* Philosopher actions and routine */
void			*philosopher_routine(void *arg);
bool			check_simulation_stop(t_philosopher *philo);
bool			try_pickup_forks(t_philosopher *philo);
int				try_to_eat(t_philosopher *philo);
void			update_meal_status(t_philosopher *philo);
bool			is_philosopher_full(t_philosopher *philo);

/* Status checking and printing */
void			print_status(t_philosopher *philo, const char *status);
// bool			check_death_time(t_philosopher *philo);
bool			check_philosopher_full(t_philosopher *philo);
bool			check_all_philosophers_full(t_simulation *sim);

/* Cleanup functions */
void			cleanup_simulation(t_simulation *sim);
int				cleanup_mutexes(t_simulation *sim);
int				destroy_mutexes_up_to(t_simulation *sim, int index);
void			free_sim_memory(t_simulation *sim);
int				destroy_philosopher_mutexes(t_philosopher *philo);
int				clean_philo_threads(t_simulation *sim, int philos_num);

/* Utility functions */
int				ft_error(const char *err);
bool			valid_args(char **av);
int				parse_arguments(t_simulation *sim, int ac, char **av);
int				ft_atoi(const char *str);
int				ft_isdigit(int c);
void			*ft_calloc(size_t num_elements, size_t element_size);
size_t			ft_strlen(const char *str);
void			ft_usleep(long microseconds);
long			get_current_time(void);

#endif
