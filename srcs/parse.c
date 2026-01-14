/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:18:34 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/14 02:05:30 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

/*typedef struct s_point
{
	int	x;
	int	y;
	int	z;
	int	color;
}	t_point;

typedef struct s_fdf
{
	t_point	**map;
	int		col;
	int		line;
	void	*mlx;
	void	*win;
}	t_fdf;
*/

parse_line(char *line)
{
}

void	parse_point(char *str, t_point *p)
{
}

t_fdf	get_data(int fd)
{
	t_fdf	data;
	char	*line;
	char	**parsed_line;
	int		i;

	line = get_next_line(fd);
	parsed_line = ft_split()

	data.line = 
	data.col =
}
