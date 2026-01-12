/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:18:48 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/13 00:02:14 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/fdf.h"

int	main(int ac, char **av)
{
	t_fdf	fdf;

	if (ac != 2)
		return (ERROR);
	fdf = get_struct(av[1]);
	if (!(fdf.height))
		return (ERROR);
	return (0);
}
