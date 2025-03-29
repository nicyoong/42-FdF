#include "fdf.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
		exit_error("Usage: ./fdf <map.fdf>");
	parse_map(argv[1], &data.map);
	data.mlx = mlx_init();
	data.win_width = 1500;
	data.win_height = 1500;
	data.win = mlx_new_window(data.mlx, data.win_width, data.win_height, "FDF");
	data.img = mlx_new_image(data.mlx, data.win_width, data.win_height);
	data.img_addr = mlx_get_data_addr(data.img, &data.bpp, &data.line_length, &data.endian);
	compute_projection(&data);
	scale_and_center(&data);
	draw_map(&data);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_hook(data.win, 2, 1L << 0, key_hook, &data);
	mlx_hook(data.win, 17, 0, close_hook, &data);
	mlx_loop(data.mlx);
	return (0);
}