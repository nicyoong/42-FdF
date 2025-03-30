/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 01:36:02 by nyoong            #+#    #+#             */
/*   Updated: 2025/03/30 22:04:32 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_bresenham	init_bresenham(t_point p0, t_point p1)
{
	t_bresenham	b;

	b.x0 = roundf(p0.x);
	b.y0 = roundf(p0.y);
	b.x1 = roundf(p1.x);
	b.y1 = roundf(p1.y);
	b.delta.x = abs(b.x1 - b.x0);
	b.delta.y = -abs(b.y1 - b.y0);
	if (b.x0 < b.x1)
		b.sign.x = 1;
	else
		b.sign.x = -1;
	if (b.y0 < b.y1)
		b.sign.y = 1;
	else
		b.sign.y = -1;
	b.error = b.delta.x + b.delta.y;
	return (b);
}

void	draw_bresenham_line(t_data *data, t_bresenham *b, int color)
{
	int	e2;

	while (1)
	{
		put_pixel(data, b->x0, b->y0, color);
		if (b->x0 == b->x1 && b->y0 == b->y1)
			break ;
		e2 = 2 * b->error;
		if (e2 >= b->delta.y)
		{
			b->error += b->delta.y;
			b->x0 += b->sign.x;
		}
		if (e2 <= b->delta.x)
		{
			b->error += b->delta.x;
			b->y0 += b->sign.y;
		}
	}
}

void	draw_line(t_point p0, t_point p1, t_data *data, int color)
{
	t_bresenham	b;

	b = init_bresenham(p0, p1);
	draw_bresenham_line(data, &b, color);
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
					data->screen_coords[i][j + 1],
					data,
					data->map->points[i][j].color);
			if (i < data->map->height - 1)
				draw_line(data->screen_coords[i][j],
					data->screen_coords[i + 1][j],
					data,
					data->map->points[i][j].color);
		}
	}
}
