/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 02:26:19 by nyoong            #+#    #+#             */
/*   Updated: 2025/03/30 22:14:54 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * The main entry point for the program.
 * 
 * This function initializes the application, sets up the graphical window using 
 * the MinilibX library, and manages the flow of the program. It handles argument
 * parsing, initializes necessary structures, and begins the rendering loop.
 * 
 * Steps:
 * 1. Validates the command-line arguments, ensuring a map file is provided.
 * 2. Parses the input map file into a data structure.
 * 3. Initializes the MinilibX library and sets up the window properties.
 * 4. Loads the map into an image buffer and computes necessary transformations 
 *    such as scaling and centering.
 * 5. Renders the map by drawing it to the window.
 * 6. Sets up event handlers for key presses and window closing.
 * 7. Starts the MinilibX event loop to handle rendering and user interactions.
 * 
 * @param argc - The number of command-line arguments.
 * @param argv - The array of command-line argument strings.
 * 
 * @return 0 - Indicates successful program execution.
 * 
 * Example usage:
 *   ./fdf map.fdf
 */
int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
		exit_error("Usage: ./fdf <map.fdf>");
	parse_map(argv[1], &data.map);
	data.mlx = mlx_init();
	data.win_width = 900;
	data.win_height = 900;
	data.win = mlx_new_window(data.mlx, data.win_width, data.win_height, "FDF");
	data.img = mlx_new_image(data.mlx, data.win_width, data.win_height);
	data.img_addr = mlx_get_data_addr(data.img, &data.bpp,
			&data.line_length, &data.endian);
	compute_projection(&data);
	scale_and_center(&data);
	draw_map(&data);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_hook(data.win, 2, 1L << 0, key_hook, &data);
	mlx_hook(data.win, 17, 0, close_hook, &data);
	mlx_loop(data.mlx);
	return (0);
}
