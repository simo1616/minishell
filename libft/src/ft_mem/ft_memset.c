/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:21:56 by mbendidi          #+#    #+#             */
/*   Updated: 2024/12/17 22:21:59 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
*	LIBRARY
*	#include <string.h>
*	DESCRIPTION
*	The memset() function writes len bytes of value c (converted to an 
*	unsigned char) to the string b.
*	PARAMETERS
*	#1. The destiny pointer in which to write.
*	#2. The character to write.
*	#3. The number of bytes to write.
*	RETURN VALUES
*	The memset() function returns its first argument.  
*/

#include "libft.h"
#include <stdlib.h>

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		((unsigned char *)b)[i] = c;
		i++;
	}
	return (b);
}
