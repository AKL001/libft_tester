/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablabib <ablabib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:20:44 by ablabib           #+#    #+#             */
/*   Updated: 2024/11/04 11:45:36 by ablabib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (nmemb > 0 && size > SIZE_MAX / nmemb)
		return (NULL);
	if (nmemb == 0 && size == 0)
		return (malloc(0));
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nmemb * size);
	return (ptr);
}
#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Test system calloc
    void *ptr1 = calloc(0, 0);
    printf("System calloc(0,0):\n");
    if (ptr1 == NULL)
        printf("Returns NULL\n");
    else
    {
        printf("Returns pointer: %p\n", ptr1);
        free(ptr1);
    }

    // Test your ft_calloc
    void *ptr2 = ft_calloc(0, 0);
    printf("\nYour ft_calloc(0,0):\n");
    if (ptr2 == NULL)
        printf("Returns NULL\n");
    else
    {
        printf("Returns pointer: %p\n", ptr2);
        free(ptr2);
    }

    // Compare behavior
    printf("\nBehavior match: %s\n",
        (ptr1 == NULL) == (ptr2 == NULL) ? "Yes" : "No");

    return (0);
}
