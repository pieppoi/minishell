/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 10:30:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 10:30:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static size_t	count_len(long nb)
{
	size_t	len;

	len = 1;
	while (nb >= 10)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

static void	fill_digits(char *str, long nb, size_t len, int is_negative)
{
	size_t	index;

	index = len + is_negative;
	while (len > 0)
	{
		str[index - 1] = (nb % 10) + '0';
		nb /= 10;
		index--;
		len--;
	}
	if (is_negative)
		str[0] = '-';
}

char	*ft_itoa(int n)
{
	long	nb;
	size_t	len;
	int		is_negative;
	char	*str;

	nb = n;
	is_negative = (nb < 0);
	if (nb < 0)
		nb = -nb;
	len = count_len(nb);
	str = (char *)malloc(len + is_negative + 1);
	if (!str)
		return (NULL);
	str[len + is_negative] = '\0';
	fill_digits(str, nb, len, is_negative);
	return (str);
}
