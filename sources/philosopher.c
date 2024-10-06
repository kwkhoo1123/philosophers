/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkah-wen <kkah-wen@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 16:15:36 by kkah-wen          #+#    #+#             */
/*   Updated: 2023/08/11 13:57:35 by kkah-wen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	print_status(philo, 0, GET_FORK_1);
	philo_sleep(philo->table, philo->table->time_to_die);
	print_status(philo, 0, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	return (NULL);
}

static void	eat_sleep_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	print_status(philo, false, GET_FORK_1);
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[1]]);
	print_status(philo, false, GET_FORK_2);
	print_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = get_time_ms();
	pthread_mutex_unlock(&philo->meal_time_lock);
	philo_sleep(philo->table, philo->table->time_to_eat);
	if (is_sim_stopped(philo->table) == false)
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		philo->times_ate += 1;
		pthread_mutex_unlock(&philo->meal_time_lock);
	}
	print_status(philo, false, SLEEPING);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[1]]);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	philo_sleep(philo->table, philo->table->time_to_sleep);
}

static void	think_routine(t_philo *philo)
{
	print_status(philo, false, THINKING);
}

void	*philosopher(void *pl)
{
	t_philo	*philo;

	philo = (t_philo *)pl;
	if (philo->table->must_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
	start_delay(philo->table->start_time);
	if (philo->table->nb_philos == 1)
		return (one_philo_routine(philo));
	else if (philo->id % 2)
		usleep(10);
	while (is_sim_stopped(philo->table) == false)
	{
		eat_sleep_routine(philo);
		think_routine(philo);
	}
	return (NULL);
}
