/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 01:36:02 by nyoong            #+#    #+#             */
/*   Updated: 2025/03/30 22:39:01 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Initializes a Bresenham line algorithm structure for drawing a line.
 * 
 * This function takes two points and computes the necessary values for 
 * Bresenham's line algorithm, including the deltas, signs, and error term. 
 * These values are used to efficiently draw a line between the two points 
 * in a rasterized environment.
 * 
 * Steps:
 * 1. Rounds the coordinates of the two points to integers.
 * 2. Calculates the deltas (differences) in the x and y directions.
 * 3. Determines the signs for both x and y based on the direction of the line.
 * 4. Computes the initial error term for the algorithm.
 * 
 * @param p0 - The starting point of the line.
 * @param p1 - The ending point of the line.
 * 
 * @return t_bresenham - A structure containing the 
 * parameters for Bresenham's algorithm.
 */

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

/**
 * Draws a line between two points using Bresenham's line algorithm.
 * 
 * This function implements Bresenham's line algorithm to draw a straight line 
 * between two points on a raster grid (like a screen). 
 * The algorithm uses integer 
 * arithmetic to incrementally compute the points along the line, 
 * avoiding floating-point 
 * operations and providing efficient pixel placement.
 * 
 * The algorithm works by calculating the error term at each step, 
 * which determines 
 * whether to move horizontally, vertically, or diagonally 
 * in order to stay closest 
 * to the ideal line. The decision to move in the x or y direction 
 * is based on comparing 
 * the error term with the deltas in the x and y directions. 
 * This method ensures that 
 * the line appears continuous and smooth on a pixel grid.
 * 
 * Steps:
 * 
 * 1. The algorithm starts by placing the 
 * initial point (`x0`, `y0`) on the screen.
 * 
 * 2. In each iteration, the error term is doubled to decide if the line should 
 *    move horizontally (`x`) or vertically (`y`), or both (diagonally).
 * 
 * 3. The pixel at the current position is drawn using `put_pixel`.
 * 
 * 4. The algorithm terminates when the endpoint (`x1`, `y1`) is reached.
 * 
 * @param data - A pointer to the data structure containing 
 * drawing context and screen.
 * @param b - A pointer to the `t_bresenham` structure 
 * containing the line's parameters.
 * @param color - The color to be used to draw the line.
 */
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

/**
 * Draws a line between two points using Bresenham's line algorithm.
 * 
 * This function initializes the Bresenham line algorithm with the two given 
 * points and then uses the algorithm to draw the line between them. The line 
 * is drawn with the specified color.
 * 
 * Steps:
 * 
 * 1. Initializes the Bresenham algorithm with the two 
 * points using `init_bresenham`.
 * 
 * 2. Draws the line by calling `draw_bresenham_line` with the initialized 
 *    Bresenham structure and the specified color.
 * 
 * @param p0 - The starting point of the line.
 * @param p1 - The ending point of the line.
 * @param data - A pointer to the data structure containing drawing context.
 * @param color - The color of the line to be drawn.
 */
void	draw_line(t_point p0, t_point p1, t_data *data, int color)
{
	t_bresenham	b;

	b = init_bresenham(p0, p1);
	draw_bresenham_line(data, &b, color);
}

/**
 * Draws the map by connecting the projected points with lines.
 * 
 * This function iterates over each point in the map's 2D grid and draws 
 * lines between adjacent points. It connects points horizontally and 
 * vertically, using the `draw_line` function to draw the lines. The color 
 * of each line is determined by the corresponding point's color.
 * 
 * Steps:
 * 
 * 1. Iterates through each row and column of the map's projected points.
 * 
 * 2. For each point, draws a horizontal line to the next point 
 * (if not the last column).
 * 
 * 3. For each point, draws a vertical line to the next point 
 * (if not the last row).
 * 
 * @param data - A pointer to the data structure containing the map,
 *  screen coordinates, and other information.
 */
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
