/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 01:36:02 by nyoong            #+#    #+#             */
/*   Updated: 2025/03/30 02:10:43 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	init_line_params(t_line *l, t_points_int *points)
{
	l->dx = abs(points->x1 - points->x0);
	l->dy = -abs(points->y1 - points->y0);
	l->x = points->x0;
	l->y = points->y0;

	if (points->x0 < points->x1)
		l->sx = 1;
	else
		l->sx = -1;

	if (points->y0 < points->y1)
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

void draw_line(t_point p0, t_point p1, t_data *data, int color)
{
	t_line line;
	t_points_int points;

	points.x0 = roundf(p0.x);
	points.y0 = roundf(p0.y);
	points.x1 = roundf(p1.x);
	points.y1 = roundf(p1.y);

	init_line_params(&line, &points);
	line.target_x = points.x1;  // Added to t_line struct
	line.target_y = points.y1;
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
