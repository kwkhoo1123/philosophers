/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkah-wen <kkah-wen@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 21:15:49 by kkah-wen          #+#    #+#             */
/*   Updated: 2023/08/09 15:35:24 by kkah-wen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	sim_stop_flag(t_table *table, int state)
{
	pthread_mutex_lock(&table->sim_stop_lock);
		table->sim_stop = state;
	pthread_mutex_unlock(&table->sim_stop_lock);
}

static int	philo_death_confirm(t_philo *philo)
{
	time_t	time;

	time = get_time_ms();
	if ((time - philo->last_meal) > philo->table->time_to_die)
	{
		sim_stop_flag(philo->table, 1);
		print_status(philo, 1, DIED);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (1);
	}
	return (0);
}

static int	end_flag_triggered(t_table *table)
{
	int	i;
	int	all_must_eat_accomplish;

	all_must_eat_accomplish = 1;
	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_lock(&table->philos[i]->meal_time_lock);
		if (philo_death_confirm(table->philos[i]))
			return (1);
		if (table->must_eat_count != -1)
			if (table->philos[i]->times_ate < table->must_eat_count)
				all_must_eat_accomplish = 0;
		pthread_mutex_unlock(&table->philos[i]->meal_time_lock);
		i++;
	}
	if (table->must_eat_count != -1 && all_must_eat_accomplish)
	{
		sim_stop_flag(table, 1);
		return (1);
	}
	return (0);
}

int	is_sim_stopped(t_table *table)
{
	int	flag;

	flag = 0;
	pthread_mutex_lock(&table->sim_stop_lock);
	if (table->sim_stop == 1)
		flag = 1;
	pthread_mutex_unlock(&table->sim_stop_lock);
	return (flag);
}

void	*death_checker(void *data)
{
	t_table			*table;

	table = (t_table *)data;
	sim_stop_flag(table, 0);
	start_delay(table->start_time);
	while (1)
	{
		if (end_flag_triggered(table) == true)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
