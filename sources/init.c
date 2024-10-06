/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkah-wen <kkah-wen@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 19:07:08 by kkah-wen          #+#    #+#             */
/*   Updated: 2023/08/09 14:36:30 by kkah-wen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* 
**	normal init:
**	pid0: fork 0, fork 1
**	pid1: fork 1, fork 2
**	pid2: fork 2, fork 0
**	deadlock could happen if when they take their first fork, then wait 2nd fork
**
**	force swapping even numbered philos hand:
**	pid0: fork 1, fork 0
**	pid1: fork 1, fork 2
**	pid2: fork 0, fork 2
**	pid0 take f1, pid1 waits f1, pid2 takes f0;
**	pid0 wait f0, pid1 continue waiting pid0's f1, 
**		pid2 finish eating, pid0 takes f0;
**	pid0 finish eating, pid1 starts eating; 
*/
pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * table->nb_philos);
	if (!forks)
	{
		error_msg("malloc", NULL);
		return (0);
	}
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
		{
			error_msg("mutex init", NULL);
			return (0);
		}
		i++;
	}
	return (forks);
}

int	init_global_mutexes(t_table *table)
{
	table->fork_locks = init_forks(table);
	if (!table->fork_locks)
		return (0);
	if (pthread_mutex_init(&table->sim_stop_lock, 0) != 0)
		return (error_msg("mutex", table));
	if (pthread_mutex_init(&table->print_lock, 0) != 0)
		return (error_msg("mutex", table));
	return (1);
}

void	assign_forks(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->table->nb_philos;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->table->nb_philos;
		philo->fork[1] = philo->id;
	}
}

t_philo	**init_philosophers(t_table *table)
{
	t_philo	**philos;
	int		i;

	philos = malloc(sizeof(t_philo) * table->nb_philos);
	if (!philos)
		return (0);
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (0);
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
			return (0);
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

t_table	*init_table(int ac, char **av)
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (0);
	table->nb_philos = ft_atoi_long(av[1]);
	table->time_to_die = ft_atoi_long(av[2]);
	table->time_to_eat = ft_atoi_long(av[3]);
	table->time_to_sleep = ft_atoi_long(av[4]);
	table->must_eat_count = -1;
	if (ac == 6)
		table->must_eat_count = ft_atoi_long(av[5]);
	table->philos = init_philosophers(table);
	if (!table->philos)
		return (NULL);
	if (!init_global_mutexes(table))
		return (NULL);
	table->sim_stop = 0;
	return (table);
}
