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
	int		i;

	lines = NULL;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_error("Failed to open file");
	while (get_next_line(fd, &line) > 0)
		ft_lstadd_back(&lines, ft_lstnew(line));
	close(fd);
	*map = malloc(sizeof(t_map));
	(*map)->height = ft_lstsize(lines);
	(*map)->width = count_words((char *)lines->content, ' ');
	(*map)->z_values = malloc(sizeof(int *) * (*map)->height);
	i = 0;
	while (lines)
	{
		(*map)->z_values[i] = malloc(sizeof(int) * (*map)->width);
		parse_line((char *)lines->content, (*map)->z_values[i++]);
		lines = lines->next;
	}
}

