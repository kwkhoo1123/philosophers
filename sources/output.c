/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkah-wen <kkah-wen@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 21:10:10 by kkah-wen          #+#    #+#             */
/*   Updated: 2023/08/08 22:09:12 by kkah-wen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	printf_status(t_philo *philo, char *str)
{
	long	time;

	time = get_time_ms() - philo->table->start_time;
	printf("%08ld %d %s\n", time, philo->id + 1, str);
}

void	print_status(t_philo *philo, int death, t_status status)
{
	pthread_mutex_lock(&philo->table->print_lock);
	if (is_sim_stopped(philo->table) && death == 0)
	{
		pthread_mutex_unlock(&philo->table->print_lock);
		return ;
	}
	if (status == DIED)
		printf_status(philo, "died");
	else if (status == EATING)
		printf_status(philo, "is eating");
	else if (status == SLEEPING)
		printf_status(philo, "is sleeping");
	else if (status == THINKING)
		printf_status(philo, "is thinking");
	else if (status == GET_FORK_1 || status == GET_FORK_2)
		printf_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->table->print_lock);
}
