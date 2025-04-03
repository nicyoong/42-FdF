/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 01:34:13 by nyoong            #+#    #+#             */
/*   Updated: 2025/04/03 22:01:28 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Handles key press events for the application.
 * 
 * This function listens for key presses and performs actions based on the 
 * keycode received. In this case, it checks if the Escape key (keycode 65307) 
 * is pressed. If so, it cleans up resources by destroying the image and 
 * window, and then exits the program.
 * 
 * The Escape key (ESC) is commonly used to quit or exit an application, 
 * and this function facilitates that behavior by cleaning up memory and 
 * terminating the program gracefully.
 * 
 * Steps:
 * 
 * 1. Checks if the Escape key (keycode 65307) is pressed.
 * 
 * 2. If the Escape key is pressed, destroys the image and window created by 
 *    the `mlx` library.
 * 
 * 3. Exits the program with status 0, indicating normal termination.
 * 
 * @param keycode - The keycode of the key that was pressed.
 * @param data - A pointer to the data structure containing 
 * relevant application data.
 * 
 * @return int - Always returns 0 to indicate successful key handling.
 */
int	key_hook(int keycode, t_data *data)
{
	if (keycode == 65307)
	{
		mlx_destroy_image(data->mlx, data->img);
		mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		mlx_loop_end(data->mlx);
		free(data->mlx);
		free_coords(data);
		free_map(data->map);
		free(data);
		exit(0);
	}
	return (0);
}

/**
 * Handles the window close event and exits the program.
 * 
 * This function is called when the user closes the application window. It 
 * ensures that resources such as the image and window created by the `mlx` 
 * library are properly cleaned up before exiting the program. It then 
 * terminates the program gracefully.
 * 
 * Steps:
 * 
 * 1. Destroys the image created by the `mlx` library to 
 * free the associated memory.
 * 
 * 2. Destroys the window created by the `mlx` library to 
 * free the associated memory.
 * 
 * 3. Exits the program with status 0, indicating normal termination.
 * 
 * @param data - A pointer to the data structure containing 
 * relevant application data.
 * 
 * @return int - Always returns 0 to indicate successful handling 
 * of the close event.
 */
int	close_hook(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	mlx_loop_end(data->mlx);
	free(data->mlx);
	free_coords(data);
	free_map(data->map);
	free(data);
	exit(0);
	return (0);
}
