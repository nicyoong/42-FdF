/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdfparser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 01:43:36 by nyoong            #+#    #+#             */
/*   Updated: 2025/03/30 22:26:23 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Reads the lines from a map file and returns them as a linked list.
 * 
 * This function opens the specified file, reads each line, and stores them
 * in a linked list. It handles errors such as failure to open the file or
 * an empty map file. Each line from the file is added to the linked list 
 * dynamically, and the file is closed once reading is completed.
 * 
 * Steps:
 * 1. Opens the specified map file for reading.
 * 
 * 2. Reads each line from the file using `get_next_line`.
 * 
 * 3. Creates a new node for each line and adds it to the linked list.
 * 
 * 4. Closes the file after reading all lines.
 * 
 * 5. Checks for an empty map file and handles errors accordingly.
 * 
 * @param filename - The name of the map file to be read.
 * 
 * @return t_list* - A linked list containing the lines from the map file.
 * 
 * @throws exit_error - If the file cannot be opened
 *  or if the map file is empty.
 */
t_list	*read_map_lines(char *filename)
{
	int		fd;
	t_list	*lines;
	char	*line;
	t_list	*node;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_error("Failed to open file");
	lines = NULL;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		node = ft_lstnew(line);
		if (!node)
			handle_node_allocation_failure(line);
		ft_lstadd_back(&lines, node);
	}
	close(fd);
	if (!lines)
		exit_error("Empty map file");
	return (lines);
}

/**
 * Initializes a map structure based on the provided list of lines.
 * 
 * This function allocates memory for the map structure and initializes its
 * properties, such as height and width, based on the number of lines in the 
 * provided list. 
 * It also allocates memory for storing the map's points (colors).
 * The function checks for memory allocation failures and exits with an error
 *  message if any allocation fails.
 * 
 * Steps:
 * 
 * 1. Allocates memory for the `t_map` structure.
 * 
 * 2. Sets the `height` of the map to the number of lines in the list.
 * 
 * 3. Sets the `width` of the map by counting the number of 
 * words in the first line.
 * 
 * 4. Allocates memory for the `points` array, which will store color information
 *  for each point in the map.
 * 
 * @param lines - A linked list of strings representing the map lines.
 * 
 * @return t_map* - A pointer to the newly initialized map structure.
 * 
 * @throws exit_error - If memory allocation for the map or points fails.
 */
t_map	*init_map(t_list *lines)
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		exit_error("Failed to allocate map");
	map->height = ft_lstsize(lines);
	map->width = count_words((char *)lines->content, ' ');
	map->points = malloc(sizeof(t_color *) * map->height);
	if (!map->points)
		exit_error("Failed to allocate map points");
	return (map);
}

/**
 * Parses a line from the map and stores the corresponding points and colors.
 * 
 * This function splits a line of map data 
 * into individual tokens, then further
 * splits each token into its `z` (height) 
 * and `color` components. The `z` value 
 * is stored as an integer, and the color 
 * is converted from a hexadecimal string 
 * (if present) into an integer. The function 
 * populates the `points_row` array 
 * with the parsed `z` and `color` values.
 * 
 * Steps:
 * 
 * 1. Splits the line into tokens based on spaces.
 * 
 * 2. For each token:
 * 
 *    - Splits the token further by a comma to separate the
 *  `z` value and the color.
 * 
 *    - Converts the `z` value to an integer and stores it.
 * 
 *    - If a color is provided, it converts the hexadecimal string 
 * to an integer and stores it; 
 * 
 *    - otherwise, the default color `0xFFFFFF` (white) is used.
 * 
 * 3. Frees the allocated memory for temporary strings (`tokens` and `parts`).
 * 
 * @param line - A string representing a line of the map, c
 * ontaining space-separated tokens.
 * @param points_row - An array of `t_color` 
 * structures to store the parsed `z` and `color` values.
 */
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

/**
 * Parses the map lines and stores the parsed data into the map's points array.
 * 
 * This function iterates over each line in the provided list of map lines,
 * allocating memory for each row of points and parsing the line data. Each 
 * line is parsed into `t_color` structures, where each point's `z` value 
 * and color are stored. The function handles memory allocation for each row
 * of points and ensures proper handling of map data.
 * 
 * Steps:
 * 
 * 1. Iterates through the list of map lines.
 * 
 * 2. Allocates memory for the `points` array in the map for each row.
 * 
 * 3. Parses each line into the corresponding `t_color` values for the row.
 * 
 * 4. Frees the memory for the line content once it's processed.
 * 
 * @param map - The map structure that will store the parsed points.
 * @param lines - A linked list of map lines, where each line contains 
 * data for a row of points.
 * 
 * @throws exit_error - If memory allocation for a row of points fails.
 */
void	parse_and_store_lines(t_map *map, t_list *lines)
{
	t_list	*current;
	int		i;

	i = 0;
	current = lines;
	while (current)
	{
		map->points[i] = malloc(sizeof(t_color) * map->width);
		if (!map->points[i])
			exit_error("Failed to allocate row");
		parse_line((char *)current->content, map->points[i++]);
		free(current->content);
		current = current->next;
	}
}

/**
 * Parses the map file and initializes the map data structure.
 * 
 * This function is responsible for reading the map from the provided file,
 * initializing a map structure, and storing the map data. It first reads the
 * map lines from the file, then initializes the map structure. Finally, it
 * parses the individual lines and stores the parsed data into the map.
 * 
 * Steps:
 * 
 * 1. Reads the map lines from the file using `read_map_lines`.
 * 
 * 2. Initializes the map data structure using `init_map`.
 * 
 * 3. Parses the map lines and stores the data into the map using 
 * `parse_and_store_lines`.
 * 
 * 4. Cleans up the list of lines after processing using `ft_lstclear`.
 * 
 * @param filename - The name of the file containing the map to be parsed.
 * @param map - A pointer to the map structure that will store the parsed data.
 */
void	parse_map(char *filename, t_map **map)
{
	t_list	*lines;

	lines = read_map_lines(filename);
	*map = init_map(lines);
	parse_and_store_lines(*map, lines);
	ft_lstclear(&lines, NULL);
}
