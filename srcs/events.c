/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:14:32 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/15 19:26:57 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

int	close_window(t_fdf *data)
{
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
	if (data->map)
		free_map(data->map, data->height - 1);
	exit(0);
	return (1);
}

int	expose_hook(t_fdf *data)
{
	if (data && data->mlx && data->win && data->img)
		mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return (0);
}

static void	check_keycode(int k, t_fdf *data)
{
	if (k == 65307)
		close_window(data);
	if (k == 61)
		data->zoom += 1;
	if (k == 45)
		data->zoom -= 1;
	if (data->zoom < 1)
		data->zoom = 1;
	if (k == 119)
		data->offset_y += 10;
	if (k == 97)
		data->offset_x += 10;
	if (k == 115)
		data->offset_y -= 10;
	if (k == 100)
		data->offset_x -= 10;
}
int	key_hook(int k, t_fdf *data)
{
	check_keycode(k, data);
	if (k == 61 || k == 45 || k == 119 || k == 97
		|| k == 115 || k == 100)
	{
		ft_memset(data->ad, 0, WIDTH * HEIGHT * 4);
		render(*data);
		mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	}
	return (0);
}
