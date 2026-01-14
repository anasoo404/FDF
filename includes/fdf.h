/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:03:07 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/14 20:26:28 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <math.h>
# include "mlx.h"
# include "libft.h"
# include "get_next_line.h"
# include "ft_printf.h"

typedef struct s_point
{
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

#endif
