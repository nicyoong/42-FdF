/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scaling.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 01:56:32 by nyoong            #+#    #+#             */
/*   Updated: 2025/03/30 22:34:11 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Finds the bounding coordinates of the map's projected points.
 * 
 * This function iterates through the map's 2D projected points 
 * and computes the 
 * minimum and maximum values for both the x and y coordinates. These bounds 
 * are used for scaling and centering the map within the window, ensuring that 
 * the entire map fits within the screen's dimensions.
 * 
 * Steps:
 * 
 * 1. Initializes the bounding values (`x_min`, `x_max`, `y_min`, `y_max`) 
 * to extreme values.
 * 
 * 2. Iterates through all the map's projected points and updates 
 * the bounds to reflect the 
 *    minimum and maximum x and y coordinates.
 * 
 * 3. Returns the computed bounds.
 * 
 * @param data - A pointer to the data structure containing 
 * the map and screen coordinates.
 * 
 * @return t_bounds - A structure containing the minimum and 
 * maximum x and y coordinates of the map.
 */
t_bounds	find_bounds(t_data *data)
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

/**
 * Calculates the scale factor to fit the map within the window.
 * 
 * This function computes the scale factor based on the map's bounding box 
 * and the window dimensions. It ensures that the map fits within the window 
 * by considering the width and height of the map and adjusting the scale 
 * proportionally. The function returns the smallest scale factor between 
 * the x and y axes to maintain the aspect ratio and avoid distortion.
 * 
 * Steps:
 * 
 * 1. Computes the horizontal span (`x_span`) and vertical span (`y_span`) 
 *    of the map based on the bounding coordinates.
 * 
 * 2. Calculates the scale factor for the x-axis and y-axis by dividing the 
 *    window size (minus padding) by the respective spans.
 * 
 * 3. Returns the smaller of the two scale factors to ensure the map fits 
 *    within the window without distortion.
 * 
 * @param data - A pointer to the data structure containing the 
 * window dimensions.
 * @param bounds - A structure containing the minimum and maximum 
 * x and y coordinates of the map.
 * 
 * @return float - The scale factor that will be used to scale 
 * the map to fit within the window.
 */
float	get_scale_factor(t_data *data, t_bounds bounds)
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

/**
 * Applies scaling and centers the map's projected points within the window.
 * 
 * This function scales the map's projected points based on the provided scale 
 * factor, and then centers them within the window. It ensures that the map 
 * is proportionally scaled and positioned in the center of the screen, taking 
 * into account the map's bounds and the window size.
 * 
 * Steps:
 * 
 * 1. Calculates the center of the map based on its bounding coordinates.
 * 
 * 2. Scales each point by the given scale factor, adjusting the position 
 *    relative to the center.
 * 
 * 3. Moves the map so that it is centered within the window.
 * 
 * @param data - A pointer to the data structure containing the map 
 * and screen coordinates.
 * @param scale - The scale factor to apply to the map's projected points.
 * @param bounds - A structure containing the minimum and maximum 
 * x and y coordinates of the map.
 */
void	apply_scaling(t_data *data, float scale, t_bounds bounds)
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

/**
 * Scales and centers the map for proper visualization on the screen.
 * 
 * This function adjusts the map's scale and 
 * position to ensure that the entire map 
 * fits within the window and is properly centered. 
 * It first computes the map's bounds 
 * (the minimum and maximum x, y, and z values), 
 * then calculates a scale factor to 
 * fit the map within the screen dimensions. 
 * Finally, it applies the scaling and centering 
 * transformations to the map's points.
 * 
 * Steps:
 * 1. Finds the bounds of the map using `find_bounds`, 
 * which determines the minimum and maximum 
 *    x, y, and z values of the map points.
 * 2. Calculates the scale factor using `get_scale_factor`, 
 * based on the map's size and the screen size.
 * 3. Applies the scaling and centering transformations 
 * to the map using `apply_scaling`.
 * 
 * @param data - A pointer to the data structure that 
 * contains the map and other related information.
 */
void	scale_and_center(t_data *data)
{
	t_bounds	bounds;
	float		scale;

	bounds = find_bounds(data);
	scale = get_scale_factor(data, bounds);
	apply_scaling(data, scale, bounds);
}
