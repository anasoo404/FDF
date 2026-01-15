/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:18:48 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/15 19:28:15 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/fdf.h"

static void	apply_events(t_fdf *data)
{
	mlx_key_hook(data->win, key_hook, data);
	mlx_hook(data->win, 17, 0, close_window, data);
	mlx_expose_hook(data->win, expose_hook, data);
}

static int	new_image(t_fdf *data)
{
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
		return (free_map(data->map, data->height - 1), write(2, "Error\n", 6));
	data->ad = mlx_get_data_addr(data->img, &data->bpp, &data->s_l, &data->edn);
	if (!data->ad)
		return (free_map(data->map, data->height - 1), write(2, "Error\n", 6));
	data->zoom = 1;
	data->offset_x = WIDTH / 2;
	data->offset_y = HEIGHT / 4;
	return (0);
}

static int	initialize_display(t_fdf *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		return (free_map(data->map, data->height - 1), write(2, "Error\n", 6));
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "FDF");
	if (!data->win)
		return (free_map(data->map, data->height - 1), write(2, "Error\n", 6));
	return (0);
}

int	main(int ac, char **av)
{
	t_fdf	data;
	int		fd;

	if (ac != 2)
		return (ERROR);
	fd = open(av[1], O_RDONLY);
	if (fd < 0)
		return (write(2, "Error\n", 6));
	close(fd);
	data = parse_data(av[1]);
	if (data.height == 0)
		return (write(2, "Error\n", 6));
	if (initialize_display(&data))
		exit(1);
	data.img = NULL;
	if (new_image(&data))
		exit(1);
	render(data);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	apply_events(&data);
	mlx_loop(data.mlx);
	return (0);
}
