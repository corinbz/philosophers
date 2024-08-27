/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccraciun <ccraciun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:40:23 by ccraciun          #+#    #+#             */
/*   Updated: 2024/08/10 15:03:07 by ccraciun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static int ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\v' ||
			c == '\n' || c == '\f' || c == '\r');
}

int ft_atoi(const char *str)
{
	long int result;
	int sign;

	result = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		sign = (*str == '-') ? -1 : 1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (result * sign > INT_MAX)
			return (INT_MAX);
		if (result * sign < INT_MIN)
			return (INT_MIN);
		str++;
	}
	return ((int)(result * sign));
}

int ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

void *ft_calloc(size_t num_elements, size_t element_size)
{
	void *result;
	size_t total_size;
	unsigned char *p;

	total_size = num_elements * element_size;
	if (num_elements && total_size / num_elements != element_size)
		return (NULL);  // Check for overflow
	result = malloc(total_size);
	if (!result)
		return (NULL);
	p = (unsigned char *)result;
	while (total_size--)
		*p++ = 0;
	return (result);
}

size_t ft_strlen(const char *str)
{
	const char *s;

	s = str;
	while (*s)
		s++;
	return (s - str);
}
