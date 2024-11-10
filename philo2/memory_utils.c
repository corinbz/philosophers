/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corin <corin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 08:20:26 by corin             #+#    #+#             */
/*   Updated: 2024/11/09 08:21:22 by corin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

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
