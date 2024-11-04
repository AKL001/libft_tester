/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablabib <ablabib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:30:17 by ablabib           #+#    #+#             */
/*   Updated: 2024/11/04 11:27:21 by ablabib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>
void	*ft_memmove(void *dest, const void *src, size_t n)
{
	if (!dest && !src)
		return (NULL);
	if (dest < src)
		ft_memcpy(dest, src, n);
	else
	{
		while (n--)
		{
			*((unsigned char *)(dest + n)) = *((unsigned char *)(src + n));
		}
	}
	return (dest);
}
#include <stdio.h>
#include <string.h>

int main()
{
    char dest[6] = "12345";

    // Using memmove with overlap (should work correctly)
    memmove(dest + 1, dest, 4);  // Move "1234" one position right in dest
    printf("After memmove: %s\n", dest);

    // Reset dest for the memcpy test
    strcpy(dest, "12345");

    // Using memcpy with overlap (may produce unexpected results)
    memcpy(dest + 1, dest, 4);  // Copy "1234" one position right in dest
    printf("After memcpy: %s\n", dest);

    return 0;
}

