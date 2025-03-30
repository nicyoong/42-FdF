/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 01:28:14 by nyoong            #+#    #+#             */
/*   Updated: 2025/03/30 01:56:48 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	compute_projection(t_data *data)
{
	int		i;
	int		j;
	float	theta;

	theta = M_PI / 6;
	data->screen_coords = malloc(sizeof(t_point *) * data->map->height);
	i = -1;
	while (++i < data->map->height)
	{
		data->screen_coords[i] = malloc(sizeof(t_point) * data->map->width);
		j = -1;
		while (++j < data->map->width)
		{
			data->screen_coords[i][j].x = (j - i) * cos(theta);
			data->screen_coords[i][j].y = (j + i) * sin(theta)
				- data->map->points[i][j].z;
		}
	}
}

void	put_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < data->win_width && y >= 0 && y < data->win_height)
	{
		dst = data->img_addr + (y * data->line_length + x * (data->bpp / 8));
		*(unsigned int *)dst = color;
	}
}
