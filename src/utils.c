/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:40:23 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/03 15:58:03 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static int	ft_isspace(int c)
{
	if ((c == ' ' || c == '\t' || c == '\v'
			|| c == '\n' || c == '\f' || c == '\r'))
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;
	int	index;

	result = 0;
	sign = 1;
	index = 0;
	while (ft_isspace(str[index]))
		index++;
	if (str[index] == '-')
	{
		sign = -1;
		index++;
	}
	else if (str[index] == '+')
		index++;
	while (str[index] >= '0' && str[index] <= '9')
	{
		result = result * 10 + str[index] - '0';
		index++;
	}
	return (result * sign);
}
int	ft_isdigit(int s)
{
	if (s <= '9' && s >= '0')
		return (1);
	return (0);
}

void	*ft_calloc(size_t num_elements, size_t element_size)
{
	void			*result;
	unsigned char	*p;
	size_t			total_size;

	total_size = num_elements * element_size;
	result = malloc(total_size);
	if (!result)
		return (NULL);
	p = (unsigned char *)result;
	while (total_size != 0)
	{
		*p = '\0';
		p++;
		total_size--;
	}
	return (result);
}

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (*str)
	{
		str++;
		len++;
	}
	return (len);
}