/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdfparser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 01:43:36 by nyoong            #+#    #+#             */
/*   Updated: 2025/03/30 02:27:49 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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
	while ((line = get_next_line(fd)) != NULL)
		ft_lstadd_back(&lines, ft_lstnew(line));
	close(fd);
	if (!lines)
		exit_error("Empty map file");
	*map = malloc(sizeof(t_map));
	(*map)->height = ft_lstsize(lines);
	(*map)->width = count_words((char *)lines->content, ' ');
	(*map)->points = malloc(sizeof(t_color *) * (*map)->height);
	i = 0;
	current = lines;
	while (current)
	{
		(*map)->points[i] = malloc(sizeof(t_color) * (*map)->width);
		parse_line((char *)current->content, (*map)->points[i++]);
		free(current->content);
		current = current->next;
	}
	ft_lstclear(&lines, NULL);
}

void	parse_line(char *line, t_color *points_row)
{
	char	**tokens;
	char	**parts;
	int		i;

	tokens = ft_split(line, ' ');
	i = 0;
	while (tokens[i])
	{
		parts = ft_split(tokens[i], ',');
		points_row[i].z = ft_atoi(parts[0]);
		if (parts[1])
			points_row[i].color = ft_atoi_base(parts[1], 16);
		else
			points_row[i].color = 0xFFFFFF;
		free_split(parts);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
