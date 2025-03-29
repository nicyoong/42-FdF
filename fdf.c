#include "fdf.h"

void	exit_error(char *msg)
{
	ft_printf("Error: %s\n", msg);
	exit(1);
}

void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

int	count_words(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

void	parse_map(char *filename, t_map **map)
{
	int		fd;
	char	*line;
	t_list	*lines;
	t_list	*current;
	int		i;

	lines = NULL;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_error("Failed to open file");
	
	// Read lines with get_next_line
	while ((line = get_next_line(fd)) != NULL)
		ft_lstadd_back(&lines, ft_lstnew(line));
	close(fd);

	// Verify we read at least one line
	if (!lines)
		exit_error("Empty map file");
	
	// Allocate map structure
	*map = malloc(sizeof(t_map));
	(*map)->height = ft_lstsize(lines);
	(*map)->width = count_words((char *)lines->content, ' ');
	(*map)->z_values = malloc(sizeof(int *) * (*map)->height);

	// Process lines and free list
	i = 0;
	current = lines;
	while (current)
	{
		(*map)->z_values[i] = malloc(sizeof(int) * (*map)->width);
		parse_line((char *)current->content, (*map)->z_values[i++]);
		free(current->content); // Free the line string
		current = current->next;
	}
	
	// Free the list nodes
	ft_lstclear(&lines, NULL);
}

void	parse_line(char *line, int *z_row)
{
	char	**tokens;
	int		i;

	tokens = ft_split(line, ' ');
	i = 0;
	while (tokens[i])
	{
		z_row[i] = ft_atoi(tokens[i]);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

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
			data->screen_coords[i][j].y = (j + i) * sin(theta) - data->map->z_values[i][j];
		}
	}
}



void	scale_and_center(t_data *data)
{
	int		i;
	int		j;
	float	min_max[4];
	float	scale;

	min_max[0] = INFINITY;
	min_max[1] = -INFINITY;
	min_max[2] = INFINITY;
	min_max[3] = -INFINITY;
	i = -1;
	while (++i < data->map->height)
	{
		j = -1;
		while (++j < data->map->width)
		{
			min_max[0] = fmin(min_max[0], data->screen_coords[i][j].x);
			min_max[1] = fmax(min_max[1], data->screen_coords[i][j].x);
			min_max[2] = fmin(min_max[2], data->screen_coords[i][j].y);
			min_max[3] = fmax(min_max[3], data->screen_coords[i][j].y);
		}
	}
	scale = fmin((data->win_width - 40) / (min_max[1] - min_max[0]),
		(data->win_height - 40) / (min_max[3] - min_max[2]));
	i = -1;
	while (++i < data->map->height)
	{
		j = -1;
		while (++j < data->map->width)
		{
			data->screen_coords[i][j].x = (data->screen_coords[i][j].x - (min_max[0] + min_max[1]) / 2) * scale + data->win_width / 2;
			data->screen_coords[i][j].y = (data->screen_coords[i][j].y - (min_max[2] + min_max[3]) / 2) * scale + data->win_height / 2;
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

void	draw_line(t_point p0, t_point p1, t_data *data)
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
		put_pixel(data, x0, y0, 0xFFFFFF);
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
				draw_line(data->screen_coords[i][j], data->screen_coords[i][j + 1], data);
			if (i < data->map->height - 1)
				draw_line(data->screen_coords[i][j], data->screen_coords[i + 1][j], data);
		}
	}
}

int	key_hook(int keycode, t_data *data)
{
	if (keycode == 65307)
	{
		mlx_destroy_image(data->mlx, data->img);
		mlx_destroy_window(data->mlx, data->win);
		exit(0);
	}
	return (0);
}

int	close_hook(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	exit(0);
	return (0);
}

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