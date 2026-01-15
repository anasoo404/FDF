/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:18:38 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/15 19:27:16 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	project_iso(t_point *p, t_fdf data)
{
	p->screen_x = (p->x - p->y) * cos(0.523599);
	p->screen_y = (p->x + p->y) * sin(0.523599) - p->z;
	p->screen_x = p->screen_x * data.zoom + data.offset_x;
	p->screen_y = p->screen_y * data.zoom + data.offset_y;
}
