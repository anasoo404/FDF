/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:03:07 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/13 22:41:39 by asmaili          ###   ########.fr       */
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

typedef struct s_point
{
	int	x;
	int	y;
	int	z;
	int	color;
}	t_point;

typedef struct s_fdf
{
	t_point	**map;// Tableau 2D de points
	int		width;// 5 (nb colonnes)
	int		height;// 5 (nb lignes)
	void	*mlx;
	void	*win;
}	t_fdf;

/*parse*/
t_fdf	get_struct(char *av);
/*utils*/
void	free_map(t_fdf fdf);
int		is_num(int c);
int		is_hex(char *str, int start, int *index);
int		ft_atoi_base(char *str, int base);

#endif
