/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkah-wen <kkah-wen@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 20:09:10 by kkah-wen          #+#    #+#             */
/*   Updated: 2023/08/08 16:21:00 by kkah-wen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_sim(t_table *table)
{
	int	i;

	i = 0;
	table->start_time = get_time_ms() + (table->nb_philos * 20);
	while (i < table->nb_philos)
	{
		if (pthread_create(&table->philos[i]->thread, NULL,
				&philosopher, table->philos[i]) != 0)
			return (error_msg("thread creation", table));
		i++;
	}
	if (table->nb_philos > 1)
	{
		if (pthread_create(&table->death_checker, NULL,
				&death_checker, table) != 0)
			return (error_msg("thread creation", table));
	}
	return (1);
}

static void	stop_sim(t_table	*table)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_join(table->philos[i]->thread, NULL);
		i++;
	}
	if (table->nb_philos > 1)
		pthread_join(table->death_checker, NULL);
	destroy_all_mutex(table);
	free_table(table);
}

int	main(int ac, char **av)
{
	t_table	*table;

	table = NULL;
	if (ac < 5 || ac > 6)
	{
		printf("Error: not 4~5 arguments.\n");
		return (1);
	}
	if (!is_valid_input(ac, av))
		return (1);
	table = init_table(ac, av);
	if (!table)
		return (1);
	if (!start_sim(table))
		return (1);
	stop_sim(table);
	return (0);
}
