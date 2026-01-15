/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:14:25 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/15 02:51:33 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static void	init_line(t_line *l, t_point p1, t_point p2)
{
	l->dx = abs(p2.screen_x - p1.screen_x);
	l->dy = abs(p2.screen_y - p1.screen_y);
	if (p1.screen_x < p2.screen_x)
		l->sx = 1;
	else
		l->sx = -1;
	if (p1.screen_y < p2.screen_y)
		l->sy = 1;
	else
		l->sy = -1;
	l->err = l->dx - l->dy;
}
static void	draw_line(t_fdf data, t_point p1, t_point p2)
{
	t_line l;

	init_line(&l, p1, p2);
	while (1)
	{
		mlx_pixel_put(data.mlx, data.win, p1.screen_x, p1.screen_y, p1.color);
		if (p1.screen_x == p2.screen_x && p1.screen_y == p2.screen_y)
			break ;
		if (l.err * 2 > -(l.dy))
		{
			l.err -= l.dy;
			p1.screen_x += l.sx;
		}
		if (l.err * 2 < l.dx)
		{
			l.err += l.dx;
			p1.screen_y += l.sy;
		}
	}
}

void	render(t_fdf data)
{
	int	x;
	int	y;

	y = 0;
	while (y < data.height)
	{
		x = 0;
		while (x < data.width)
			project_iso(&(data.map[y][x++]));
		y += 1;
	}
	y = 0;
	while (y < data.height)
	{
		x = 0;
		while (x < data.width)
		{
			if (x + 1 < data.width)
				draw_line(data, data.map[y][x], data.map[y][x + 1]);
			if (y + 1 < data.height)
				draw_line(data, data.map[y][x], data.map[y + 1][x]);
			x += 1;
		}
		y += 1;
	}
}
