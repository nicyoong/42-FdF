/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scaling.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 01:56:32 by nyoong            #+#    #+#             */
/*   Updated: 2025/03/30 02:39:03 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_bounds	find_bounds(t_data *data)
{
	t_bounds	bounds;
	int			i;
	int			j;

	i = -1;
	bounds.x_min = INFINITY;
	bounds.x_max = -INFINITY;
	bounds.y_min = INFINITY;
	bounds.y_max = -INFINITY;
	while (++i < data->map->height)
	{
		j = -1;
		while (++j < data->map->width)
		{
			bounds.x_min = fmin(bounds.x_min, data->screen_coords[i][j].x);
			bounds.x_max = fmax(bounds.x_max, data->screen_coords[i][j].x);
			bounds.y_min = fmin(bounds.y_min, data->screen_coords[i][j].y);
			bounds.y_max = fmax(bounds.y_max, data->screen_coords[i][j].y);
		}
	}
	return (bounds);
}

static float	get_scale_factor(t_data *data, t_bounds bounds)
{
	float	x_span;
	float	y_span;
	float	x_scale;
	float	y_scale;

	x_span = bounds.x_max - bounds.x_min;
	y_span = bounds.y_max - bounds.y_min;
	x_scale = (data->win_width - 40) / x_span;
	y_scale = (data->win_height - 40) / y_span;
	return (fmin(x_scale, y_scale));
}

static void	apply_scaling(t_data *data, float scale, t_bounds bounds)
{
	int		i;
	int		j;
	float	x_center;
	float	y_center;

	i = -1;
	x_center = (bounds.x_min + bounds.x_max) / 2;
	y_center = (bounds.y_min + bounds.y_max) / 2;
	while (++i < data->map->height)
	{
		j = -1;
		while (++j < data->map->width)
		{
			data->screen_coords[i][j].x = (data->screen_coords[i][j].x
					- x_center) * scale + data->win_width / 2;
			data->screen_coords[i][j].y = (data->screen_coords[i][j].y
					- y_center) * scale + data->win_height / 2;
		}
	}
}

void	scale_and_center(t_data *data)
{
	t_bounds	bounds;
	float		scale;

	bounds = find_bounds(data);
	scale = get_scale_factor(data, bounds);
	apply_scaling(data, scale, bounds);
}
