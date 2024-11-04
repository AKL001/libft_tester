/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablabib <ablabib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:30:26 by ablabib           #+#    #+#             */
/*   Updated: 2024/10/27 16:31:48 by ablabib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	s_len;

	s_len = ft_strlen(s);
	if ((char)c == '\0')
		return ((char *)(s + s_len));
	while (s_len > 0)
	{
		if (*(s + s_len - 1) == (char)c)
			return ((char *)(s + s_len - 1));
		s_len--;
	}
	return (NULL);
}
