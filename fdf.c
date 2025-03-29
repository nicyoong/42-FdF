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



