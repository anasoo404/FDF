/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:18:34 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/13 22:53:59 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"


int	is_format(char **line, int *error, int *width)
{
	int	i;
	int	start;

	i = 0;
	while ((*line)[i])
	{
		if (!(is_num((*line)[i++])))
			return (*error = 1, ERROR);
		*width += 1;
		if ((*line)[i])
		{
			if ((*line)[i] == ',')
			{
				i += 1;
				start = i;
				if (!(is_hex(line + i, start, &i)))
					return (*error = 1, ERROR);
			}
			else if ((*line)[i] != ' ' || !(is_num((*line)[i + 1])))
				return (*error = 1, ERROR);
			i += 1;
		}
	}
}

static int	get_width_and_height_by_parsing(t_fdf *fdf, int fd, char **line)
{
	int	error;
	int	width;

	error = 0;
	width = 0;
	is_format(line, &error, &width);
	if (fdf->width == 0)
		fdf->width = width;
	if (error || fdf->width != width || !width)
	{
		*line = NULL;
		fdf->height = 0;
		return (ERROR);
	}
	fdf->height += 1;
	*line = get_next_line(fd);
}

static void	fill_map(t_point *fdf, int i, int width, int fd)
{
	int		j;
	char	*line;

	line = get_next_line(fd);
	j = 0;
	while (*line)
	{
		map[j]->x = j;
		fdf->map[i][j]->z = ft_atoi(line + j);
		while (is_num(*line + j))
			line += 1;
		if (*line == ',')
		{
			i += 1;
		}
		j += 1;
	}
}

static int	create_map(t_fdf *fdf, int fd)
{
	int	i;
	int	j;

	fdf->map = malloc(fdf->height * sizeof(t_point *));
	if (!(fdf->map))
		return (fdf->height = 0, ERROR);
	i = 0;
	while (i < fdf->height)
	{
		fdf->map[i] = malloc(fdf->width * sizeof(t_point));
		if (!(fdf->map[i++]))
			return (free_map(*fdf), ERROR);
	}
	i = 0;
	while (i < fdf->height)
	{
		j = 0;
		while (j < fdf->width)
			fdf->map[i][j++]->y = i;
		fill_map(fdf, i, fdf->width, fd);
		i += 1;
	}
}

t_fdf	get_struct(char *av)
{
	int		fd;
	int		height;
	int		width;
	char	*line;
	t_fdf	fdf;

	fd = open(av, O_RDONLY);
	fdf.height = 0;
	if (fd < 0)
		return (fdf);
	fdf.width = 0;
	line = get_next_line(fd);
	while (line)
		get_width_and_height_by_parsing(&fdf, fd, &line);
	if (fdf.height == 0)
		return (fdf);
	close(fd);
	fd = open(av, O_RDONLY);
	if (fd < 0)
		return (fdf.height = 0, fdf);
	if (create_map(&fdf, fd) == ERROR)
		return (fdf.height = 0, fdf);
	close(fd);
	return (fdf);
}
