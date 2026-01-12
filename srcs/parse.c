/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:18:34 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/13 00:12:53 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"


void	is_format(char **line, int *erro, int *width)
{
	JE PREND UNE SUBSTR JUSQU A AV L ESPACE OU '\0'
	je fais les check necessaires change error a 1 si y en a une
	des qu une valeur est trouve
}

static int	get_width_and_height_by_parsing(t_fdf *fdf, int fd, char **line)
{
	int	error;
	int	width;

	error = 0;
	width = 0;
	while (**line && !error)
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

static void	fill_map(t_point *map, int i, int width)
{
	int	j;

	j = 0;
	while (j < width)
	{

	}
}

static int	create_map(t_fdf *fdf)
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
		fill_map(fdf->map, i, fdf->width);
		i += 1;
	}
	// ici je fill la map avec les donnes qu il faut
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
		return (fdf);
	create_map(&fdf);
	close(fd);
	return (fdf);
}
