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

static t_list *read_map_lines(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
        exit_error("Failed to open file");

    t_list *lines = NULL;
    char *line;

    while (1) {
		line = get_next_line(fd);
		if (!line) break;
	
		t_list *node = ft_lstnew(line);
		if (!node) {
			free(line);
			exit_error("Failed to allocate list node");
		}
		ft_lstadd_back(&lines, node);
	}
    close(fd);

    if (!lines)
        exit_error("Empty map file");
    return lines;
}

static t_map *init_map(t_list *lines) {
    t_map *map = malloc(sizeof(t_map));
    if (!map)
        exit_error("Failed to allocate map");

    map->height = ft_lstsize(lines);
    map->width = count_words((char *)lines->content, ' ');
    map->points = malloc(sizeof(t_color *) * map->height);
    if (!map->points)
        exit_error("Failed to allocate map points");

    return map;
}

static void parse_and_store_lines(t_map *map, t_list *lines) {
    t_list *current = lines;
    int i = 0;

    while (current) {
        map->points[i] = malloc(sizeof(t_color) * map->width);
        if (!map->points[i])
            exit_error("Failed to allocate row");

        parse_line((char *)current->content, map->points[i++]);
        free(current->content);
        current = current->next;
    }
}

void parse_map(char *filename, t_map **map) {
    t_list *lines = read_map_lines(filename);
    *map = init_map(lines);
    parse_and_store_lines(*map, lines);
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
