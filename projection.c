/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 01:28:14 by nyoong            #+#    #+#             */
/*   Updated: 2025/03/30 22:42:09 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Computes the 2D projection of the map's 3D points onto the screen.
 * 
 * This function takes the 3D map points (with x, y, z values) and applies 
 * a projection transformation to convert them into 2D screen coordinates. 
 * It uses an isometric projection with a fixed angle (`theta`) to project the 
 * 3D coordinates onto a 2D plane. The result is stored in the `screen_coords` 
 * array, which holds the 2D coordinates for each map point.
 * 
 * Steps:
 * 1. Allocates memory for the `screen_coords` array, 
 * which holds the 2D coordinates.
 * 
 * 2. Iterates over each map point and applies the 
 * isometric projection formula:
 *    - The x-coordinate is calculated using a cosine function.
 *    - The y-coordinate is calculated using a sine function, 
 * adjusted by the `z` value.
 * 
 * 3. Stores the projected coordinates in the `screen_coords` array.
 * 
 * @param data - A pointer to the data structure that 
 * contains the map and other related information.
 */
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

/**
 * Places a pixel at the specified coordinates in the image.
 * 
 * This function writes a pixel with the specified color to the image at the 
 * given `(x, y)` coordinates. It ensures that the coordinates are within 
 * the boundaries of the window before attempting to write the pixel. The 
 * function directly manipulates the image's memory buffer using the `mlx` 
 * library's pixel data address.
 * 
 * Steps:
 * 
 * 1. Checks if the specified coordinates `(x, y)` are within the window's 
 *    bounds (`0 <= x < win_width` and `0 <= y < win_height`).
 * 
 * 2. Computes the address of the pixel in the image's memory using the 
 *    provided coordinates.
 * 
 * 3. Writes the pixel color to the computed memory address.
 * 
 * @param data - A pointer to the data structure 
 * containing the image and its properties.
 * @param x - The x-coordinate of the pixel to be placed.
 * @param y - The y-coordinate of the pixel to be placed.
 * @param color - The color of the pixel to be placed (in hexadecimal format).
 */
void	put_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < data->win_width && y >= 0 && y < data->win_height)
	{
		dst = data->img_addr + (y * data->line_length + x * (data->bpp / 8));
		*(unsigned int *)dst = color;
	}
}
