/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:18:42 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/13 22:47:48 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	free_map(t_fdf fdf)
{
	int	i;

	i = 0;
	while (i < fdf.height)
		free(fdf.map[i++]);
	free(fdf.map);
}

int	is_num(int c)
{
	if (c < '0' || c > '9')
		return (0);
	return (1);
}

static void	fill_base(char *base)
{
	int		i;
	char	c;

	i = 0;
	while (i < 16)
	{
		c = '0';
		while (i < 10)
			base[i++] = c++;
		c = 'A';
		base[i++] = c++;
	}
}

int	is_hex(char *str, int start, int *index)
{
	char	base[16];
	int		error;
	int		i;

	fill_base(base);
	if (str[0] != '0' || (str[1] != 'x' && str[1] != 'X') || !(str[2]))
		return (ERROR);
	*index += 2;
	while (str[*index])
	{
		error = 1;
		i = 0;
		while (i < 16)
		{
			if (str[*index] == base[i++])
				break ;
		}
		if (str[*index] != base[i - 1])
			return (ERROR);
		*index += 1;
		if (*index - start > 8)
			return (ERROR);
	}
	return (1);
}

int	ft_atoi_base(char *str, int base)
{
	int	result;
	int	i;

	result = 0;
	i = 0;
	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
		i = 2;
	while (i < 8)
	{
		result *= base;
		if (str[i] >= '0' && str[i] <= '9')
			result += str[i] - '0';
		else if (str[i] >= 'a' && str[i] <= 'f')
			result += str[i] - 'a' + 10;
		else if (str[i] >= 'A' && str[i] <= 'F')
			result += str[i] - 'A' + 10;
		i++;
	}
	return (result);
}

