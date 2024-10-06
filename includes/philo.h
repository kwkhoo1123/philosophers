/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkah-wen <kkah-wen@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 20:27:10 by kkah-wen          #+#    #+#             */
/*   Updated: 2023/08/08 22:08:41 by kkah-wen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct s_philo	t_philo;

typedef struct s_table
{
	time_t			start_time;
	int				nb_philos;
	pthread_t		death_checker;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				must_eat_count;
	int				sim_stop;
	pthread_mutex_t	sim_stop_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	*fork_locks;
	t_philo			**philos;
}	t_table;

typedef struct s_philo
{
	pthread_t			thread;
	int					id;
	int					times_ate;
	int					fork[2];
	pthread_mutex_t		meal_time_lock;
	time_t				last_meal;
	t_table				*table;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GET_FORK_1 = 4,
	GET_FORK_2 = 5
}	t_status;

long	ft_atoi_long(const char *str);
int		is_valid_input(int ac, char **av);

void	destroy_all_mutex(t_table *table);
void	*free_table(t_table *table);
int		error_msg(char *str, t_table *table);

time_t	get_time_ms(void);
void	start_delay(time_t start_time);
void	philo_sleep(t_table *table, time_t sleep_time);

void	print_status(t_philo *philo, int death, t_status status);

void	*death_checker(void *data);
int		is_sim_stopped(t_table *table);

t_table	*init_table(int ac, char **av);

void	*philosopher(void *pl);

#endif