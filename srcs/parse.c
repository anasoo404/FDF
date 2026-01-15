/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:18:34 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/15 02:13:44 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static void	get_map_dimensions(int fd, t_fdf *data)
{
	char	**parsed_line;
	char	*line;
	int		count;

	parsed_line = ft_split(get_next_line(fd), ' ');
	if (!parsed_line)
		return ;
	count = 0;
	while (parsed_line[count])
	{
		free(parsed_line[count]);
		count++;
	}
	free(parsed_line);
	data->width = count;
	count = 0;
	line = get_next_line(fd);
	while (line)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	data->height = count;
}
static int	fill_t_point(t_point *p, char **parsed_line, int x, int y)
{
	char	**data;

	data = ft_split(parsed_line[x], ',');
	if (!data)
		return (free_split(parsed_line), 0);
	p->x = x;
	p->y = y;
	p->z = ft_atoi(data[0]);
	if (data[1])
		p->color = ft_atoi_base(data[1], 16);
	else
		p->color = ft_atoi_base("0xFFFFFF", 16);
	free_split(data);
	return (1);
}

static int	fill_map(t_point **map, int width, int height, int fd)
{
	int		x;
	int		y;
	char	*line;
	char	**parsed_line;

	y = 0;
	while (y < height)
	{
		line = get_next_line(fd);
		parsed_line = ft_split(line, ' ');
		free(line);
		if (!parsed_line)
			return (0);
		x = 0;
		while (x < width)
		{
			if (!(fill_t_point(&map[y][x], parsed_line, x, y)))
				return (free_split(parsed_line), 0);
			x += 1;
		}
		free_split(parsed_line);
		y += 1;
	}
	return (1);
}

static t_point	**parse_map(int fd, t_fdf data)
{
	int		i;
	t_point **map;

	map = malloc(sizeof(t_point *) * data.height);
	if (!map)
		return (NULL);
	i = 0;
	while (i < data.height)
	{
		map[i] = malloc(sizeof(t_point) * data.width);
		if (!map[i])
			return (free_map(map, i), NULL);
		i++;
	}
	if (!(fill_map(map, data.width, data.height, fd)))
		return (free_map(map, i - 1), NULL);
	return (map);
}

t_fdf	parse_data(char *fdf)
{
	t_fdf	data;
	int		fd;

	fd = open(fdf, O_RDONLY);
	if (fd < 0)
		return (data.height = 0, data);
	data.height = 0;
	get_map_dimensions(fd, &data);
	close(fd);
	if (data.height == 0)
		return (data);
	fd = open(fdf, O_RDONLY);
	if (fd < 0)
		return (data.height = 0, data);
	data.map = parse_map(fd, data);
	close(fd);
	if (!(data.map))
		return (data.height = 0, data);
	return (data);
}
