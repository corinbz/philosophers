/*	**************************************************************************	*/
/*																																																																												*/
/*																																																								:::						::::::::			*/
/*			philosopher_utils.c																																:+:						:+:				:+:			*/
/*																																																				+:+	+:+									+:+					*/
/*			By:	ccraciun	<ccraciun@student.42.fr>										+#+		+:+							+#+								*/
/*																																																+#+#+#+#+#+			+#+											*/
/*			Created:	2024/08/03	15:15:58	by	ccraciun										#+#				#+#													*/
/*			Updated:	2024/08/10	15:50:08	by	ccraciun									###			########.fr							*/
/*																																																																												*/
/*	**************************************************************************	*/

#include	"../include/philosophers.h"

void	print_status(t_philosopher	*philo,	const	char	*status)
{
				long	long	current_time	=	get_current_time();
				pthread_mutex_lock(philo->print_mutex);
				if	(!*(philo->simulation_stop))
								printf("%lld	%d	%s\n",	current_time,	philo->id,	status);
				pthread_mutex_unlock(philo->print_mutex);
}

//	Implement	a	hierarchical	resource	allocation	strategy
bool	try_pickup_forks(t_philosopher	*philo)
{
				int	first_fork	=	(philo->id	%	2	==	0)	?	
								(philo->id	-	1)	:	(philo->id	%	philo->num_philosophers);
				int	second_fork	=	(philo->id	%	2	==	0)	?	
								(philo->id	%	philo->num_philosophers)	:	(philo->id	-	1);

				pthread_mutex_lock(philo->waiter_mutex);
				if	(philo->forks_available[first_fork]	&&	philo->forks_available[second_fork])
				{
								philo->forks_available[first_fork]	=	false;
								philo->forks_available[second_fork]	=	false;
								pthread_mutex_unlock(philo->waiter_mutex);

								pthread_mutex_lock(philo->left_fork);
								print_status(philo,	"has	taken	a	fork");
								pthread_mutex_lock(philo->right_fork);
								print_status(philo,	"has	taken	a	fork");
								return	true;
				}
				pthread_mutex_unlock(philo->waiter_mutex);
				return	false;
}


int	try_to_eat(t_philosopher	*philo)
{
				if	(!try_pickup_forks(philo))
								return	0;

				long	long	eat_start	=	get_current_time();
				philo->last_meal_time	=	eat_start;
				print_status(philo,	"is	eating");
				ft_usleep(philo->time_to_eat	*	1000);
				philo->meals_eaten++;

				return_forks(philo);
				
				long	long	eat_end	=	get_current_time();
				printf("Debug:	Philosopher	%d	ate	for	%lld	ms	(started	at	%lld,	ended	at	%lld)\n",	
											philo->id,	eat_end	-	eat_start,	eat_start,	eat_end);
				
				return	1;
}
void	return_forks(t_philosopher	*philo)
{
				int	left_fork_index	=	philo->id	-	1;
				int	right_fork_index	=	philo->id	%	philo->num_philosophers;

				pthread_mutex_unlock(philo->right_fork);
				pthread_mutex_unlock(philo->left_fork);

				pthread_mutex_lock(philo->waiter_mutex);
				philo->forks_available[left_fork_index]	=	true;
				philo->forks_available[right_fork_index]	=	true;
				pthread_mutex_unlock(philo->waiter_mutex);
}
void	*philosopher_routine(void	*arg)
{
				t_philosopher	*philo	=	(t_philosopher	*)arg;
				
				if	(!philo)	return	NULL;
				
				//	Stagger	the	start	times	to	reduce	contention
				ft_usleep((philo->id	%	philo->num_philosophers)	*	1000);
				
				while	(!*(philo->simulation_stop))
				{
								if	(try_to_eat(philo))
								{
												print_status(philo,	"is	sleeping");
												ft_usleep(philo->time_to_sleep	*	1000);
												print_status(philo,	"is	thinking");
								}
								ft_usleep(1000);	//	Short	delay	before	trying	again
				}
				return	NULL;
}

