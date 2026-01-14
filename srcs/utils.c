/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:18:42 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/14 01:33:58 by asmaili          ###   ########.fr       */
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

