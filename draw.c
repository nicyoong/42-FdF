/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 01:36:02 by nyoong            #+#    #+#             */
/*   Updated: 2025/03/30 01:36:09 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.c"

void	draw_line(t_point p0, t_point p1, t_data *data, int color)
{
	t_point	delta;
	t_point	sign;
	int		error;
	int		e2;
	int		x0;
	int		y0;
	int		x1;
	int		y1;

	// Convert floating-point coordinates to integers
	x0 = roundf(p0.x);
	y0 = roundf(p0.y);
	x1 = roundf(p1.x);
	y1 = roundf(p1.y);

	delta.x = abs(x1 - x0);
	delta.y = -abs(y1 - y0);
	sign.x = x0 < x1 ? 1 : -1;
	sign.y = y0 < y1 ? 1 : -1;
	error = delta.x + delta.y;

	while (1)
	{
		put_pixel(data, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		e2 = 2 * error;
		if (e2 >= delta.y)
		{
			error += delta.y;
			x0 += sign.x;
		}
		if (e2 <= delta.x)
		{
			error += delta.x;
			y0 += sign.y;
		}
	}
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
