/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 16:02:00 by wmillett          #+#    #+#             */
/*   Updated: 2023/02/12 16:02:00 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_isdigit_base(int c, int base)
{
	if (base <= 10)
		return (c >= '0' && c <= '0' + base - 1);
	return ((c >= '0' && c <= '9') || 
			(c >= 'a' && c <= 'a' + base - 11) ||
			(c >= 'A' && c <= 'A' + base - 11));
}

static int	ft_char_to_digit(int c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'z')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'Z')
		return (c - 'A' + 10);
	return (-1);
}

long	ft_strtol(const char *str, char **endptr, int base)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	
	// Skip whitespace
	while (ft_isspace(*str))
		str++;
	
	// Handle sign
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	
	// Convert digits
	while (ft_isdigit_base(*str, base))
	{
		result = result * base + ft_char_to_digit(*str);
		str++;
	}
	
	// Set endptr if provided
	if (endptr)
		*endptr = (char *)str;
	
	return (result * sign);
} 