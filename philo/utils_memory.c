/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:40:23 by ccraciun          #+#    #+#             */
/*   Updated: 2024/10/23 09:49:56 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Allocates zeroed memory for an array
** @param num_elements: Number of elements to allocate
** @param element_size: Size of each element
** @return: Pointer to allocated memory or NULL on failure
*/
void	*ft_calloc(size_t num_elements, size_t element_size)
{
	void			*result;
	size_t			total_size;
	unsigned char	*p;

	total_size = num_elements * element_size;
	if (num_elements && total_size / num_elements != element_size)
		return (NULL);
	result = malloc(total_size);
	if (!result)
		return (NULL);
	p = (unsigned char *)result;
	while (total_size--)
		*p++ = 0;
	return (result);
}

/*
** Calculates the length of a string
** @param str: String to measure
** @return: Length of the string
*/
size_t	ft_strlen(const char *str)
{
	const char	*s;

	s = str;
	while (*s)
		s++;
	return (s - str);
}
