/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:18:48 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/15 02:25:59 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/fdf.h"

int	main(int ac, char **av)
{
	t_fdf	data;
	int		fd;

	if (ac != 2)
		return (ERROR);
	fd = open(av[1], O_RDONLY);
	if (fd < 0)
		return (write(2, "Error\n", 6));
	data = parse_data(av[1]);
	if (data.height == 0)
		return (write(2, "Error\n", 6));
	data.mlx = mlx_init();
	if (!data.mlx)
		return (write(2, "Error: mlx_init\n", 16));
	data.win = mlx_new_window(data.mlx, WIDTH, HEIGHT, "FDF");
	if (!data.win)
		return (write(2, "Error: mlx_new_window\n", 22));
	render(data);
	mlx_loop(data.mlx);
	free_map(data.map, data.height - 1);
	return (0);
}
