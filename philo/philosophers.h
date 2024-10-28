/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:14:11 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 15:40:16 by ccraciun         ###   ########.fr       */
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

# define MAX_FORK_ATTEMPTS 50
# define FORK_ATTEMPT_DELAY 1000

typedef struct s_philosopher
{
    pthread_mutex_t    *time_zero_mut;
    pthread_mutex_t    *last_meal_mut;
    pthread_mutex_t    *sim_stop_mut;
    long            time_zero;
    int                id;
    pthread_t        thread;
    pthread_mutex_t    *left_fork;
    pthread_mutex_t    *right_fork;
    int                left_fork_id;
    int                right_fork_id;
    int                meals_eaten;
    long            last_meal_time;
    _Atomic bool    *simulation_stop;
    pthread_mutex_t    *print_mutex;
    int                time_to_die;
    int                time_to_eat;
    int                time_to_sleep;
    int                num_times_to_eat;
    int                num_philosophers;
    _Atomic bool    is_full;
}    t_philosopher;

typedef struct s_simulation
{
    t_philosopher    *philosophers;
    pthread_mutex_t    *forks;
    int                num_philosophers;
    int                time_to_die;
    int                time_to_eat;
    int                time_to_sleep;
    int                num_times_to_eat;
    _Atomic bool    simulation_stop;
    pthread_mutex_t    print_mutex;
    bool            *forks_available;
}    t_simulation;

// Initialization
t_simulation    *init_simulation(int ac, char **av);
int                parse_arguments(t_simulation *sim, int ac, char **av);
int                init_mutexes(t_simulation *sim);
void            set_fork_ids(t_simulation *sim, int i);

// Simulation control
int                start_simulation(t_simulation *sim);
void            *monitor_simulation(void *arg);
void            set_starting_time(t_simulation *sim);

// Philosopher actions
void            *philosopher_routine(void *arg);
bool            try_pickup_forks(t_philosopher *philo);
int                try_to_eat(t_philosopher *philo);
void            print_status(t_philosopher *philo, const char *status);
bool            check_simulation_stop(t_philosopher *philo);

// Utility functions
int                ft_atoi(const char *str);
int                ft_isdigit(int s);
void            *ft_calloc(size_t num_elements, size_t element_size);
size_t            ft_strlen(const char *str);
int                ft_error(const char *err);
bool            valid_args(char **av);

// Time utilities
void            ft_usleep(long long time);
long long        get_current_time(void);

// Cleanup
void            cleanup_simulation(t_simulation *sim);
void            free_sim_memory(t_simulation *sim);
int                clean_philo_threads(t_simulation *sim, int philos_num);

#endif
