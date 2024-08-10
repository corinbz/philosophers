/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:14:11 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/10 15:04:06 by ccraciun         ###   ########.fr       */
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

typedef struct s_philosopher
{
    int             id;
    pthread_t       thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    int             meals_eaten;
    long long       last_meal_time;
    int             *simulation_stop;
    pthread_mutex_t *print_mutex;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             num_times_to_eat;
    pthread_mutex_t *waiter_mutex;
    bool            *forks_available;
    int             num_philosophers;
    int             is_full;
} t_philosopher;

typedef struct s_simulation
{
    t_philosopher   *philosophers;
    pthread_mutex_t *forks;
    int             num_philosophers;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             num_times_to_eat;
    int             simulation_stop;
    pthread_mutex_t print_mutex;
    pthread_mutex_t waiter_mutex;
    bool            *forks_available;
} t_simulation;

// Initialization
t_simulation    *init_simulation(int ac, char **av);

// Simulation control
int             start_simulation(t_simulation *sim);
void            *monitor_simulation(void *arg);

// Philosopher actions
void            *philosopher_routine(void *arg);
bool            try_pickup_forks(t_philosopher *philo);
void            return_forks(t_philosopher *philo);
int             try_to_eat(t_philosopher *philo);
void            print_status(t_philosopher *philo, const char *status);

// Utility functions
int             ft_atoi(const char *str);
int             ft_isdigit(int s);
void            *ft_calloc(size_t num_elements, size_t element_size);
size_t          ft_strlen(const char *str);
int             ft_error(const char *err);
bool            valid_args(char **av);

// Time utilities
void            ft_usleep(long long time);
long long       get_current_time(void);

// Cleanup
void            cleanup_simulation(t_simulation *sim);

#endif