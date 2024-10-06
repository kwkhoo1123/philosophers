/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkah-wen <kkah-wen@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 20:11:00 by kkah-wen          #+#    #+#             */
/*   Updated: 2023/08/05 18:21:16 by kkah-wen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atoi_long(const char *str)
{
	int		sign;
	long	sum;
	int		i;

	sum = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		sum = sum * 10 + str[i] - '0';
		i++;
	}
	return (sign * sum);
}

int	is_digit_only(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_input(int ac, char **av)
{
	int		i;
	long	nb;

	i = 1;
	while (i < ac)
	{
		if (!is_digit_only(av[i]))
		{
			printf("Error: non-digit detected\n");
			return (0);
		}
		nb = ft_atoi_long(av[i]);
		if (nb <= 0 || nb > 2147483647)
		{
			printf("Error: invalid number. Enter between 0 and 2147483647.\n");
			return (0);
		}
		i++;
	}
	return (1);
}
