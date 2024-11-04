/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablabib <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 10:41:24 by ablabib           #+#    #+#             */
/*   Updated: 2024/11/04 10:39:39 by ablabib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *src)
{
	size_t	i;
	size_t	nb;
	size_t	sign;

	nb = 0;
	sign = 1;
	i = 0;
	while ((src[i] >= 9 && src[i] <= 13) || src[i] == 32)
		i++;
	if (src[i] == '-' || src[i] == '+')
	{
		if (src[i + 1] == '+' || src[i + 1] == '-')
			return (0);
		else if (src[i] == '-')
			sign = -1;
		i++;
	}
	while (src[i] >= '0' && src[i] <= '9')
	{
		nb = (nb * 10) + (src[i] - '0');
		i++;
	}
	return (nb * sign);
}
