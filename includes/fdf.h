/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:03:07 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/15 02:27:27 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# define WIDTH 1920
# define HEIGHT 1080

# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <math.h>
# include "mlx.h"
# include "libft.h"
# include "get_next_line.h"
# include "ft_printf.h"

typedef struct s_line
{
	int dx;
	int dy;
	int sx;
	int sy;
	int err;
}	t_line;

typedef struct s_point
{
	int	screen_x;
	int	screen_y;
	int	x;
	int	y;
	int	z;
	int	color;
}	t_point;

typedef struct s_fdf
{
	t_point	**map;
	int		width;
	int		height;
	void	*mlx;
	void	*win;
}	t_fdf;

/*parse*/
t_fdf	parse_data(char *fdf);

/*utils*/
void	free_split(char **split);
void	free_map(t_point **map, int index_fail);
int		ft_atoi_base(char *str, int base);

/*projection*/
void	project_iso(t_point *p);

/*draw*/
void	render(t_fdf data);

#endif
