/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 01:36:02 by nyoong            #+#    #+#             */
/*   Updated: 2025/03/30 02:07:50 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	init_line_params(t_line *l, int x0, int y0, int x1, int y1)
{
	l->dx = abs(x1 - x0);
	l->dy = -abs(y1 - y0);
	l->x = x0;
	l->y = y0;

	if (x0 < x1)
		l->sx = 1;
	else
		l->sx = -1;

	if (y0 < y1)
		l->sy = 1;
	else
		l->sy = -1;

	l->err = l->dx + l->dy;
}

static void	draw_pixels(t_data *data, t_line *l, int color)
{
	int	e2;

	while (1)
	{
		put_pixel(data, l->x, l->y, color);
		if (l->x == l->sx && l->y == l->sy)
			break;
		
		e2 = 2 * l->err;
		if (e2 >= l->dy)
		{
			l->err += l->dy;
			l->x += l->sx;
		}
		if (e2 <= l->dx)
		{
			l->err += l->dx;
			l->y += l->sy;
		}
	}
}

void	draw_line(t_point p0, t_point p1, t_data *data, int color)
{
	t_line	line;
	int		x0;
	int		y0;
	int		x1;
	int		y1;

	x0 = roundf(p0.x);
	y0 = roundf(p0.y);
	x1 = roundf(p1.x);
	y1 = roundf(p1.y);
	init_line_params(&line, x0, y0, x1, y1);
	line.sx = x1;
	line.sy = y1;
	draw_pixels(data, &line, color);
}

void	draw_map(t_data *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->map->height)
	{
		j = -1;
		while (++j < data->map->width)
		{
			if (j < data->map->width - 1)
				draw_line(data->screen_coords[i][j], 
					data->screen_coords[i][j+1], 
					data, 
					data->map->points[i][j].color);

			if (i < data->map->height - 1)
				draw_line(data->screen_coords[i][j], 
					data->screen_coords[i+1][j], 
					data, 
					data->map->points[i][j].color);
		}
	}
}
