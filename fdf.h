#ifndef FDF_H
# define FDF_H

/*** INCLUDES ***/
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include "libft/libft.h"          // Your libft header
# include "minilibx-linux/mlx.h"  // Linux MiniLibX

/*** STRUCTURES ***/
typedef struct s_point
{
	float	x;
	float	y;
}	t_point;

typedef struct s_map
{
	int		height;
	int		width;
	int		**z_values;
}	t_map;

typedef struct s_data
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*img_addr;
	int		bpp;
	int		line_length;
	int		endian;
	int		win_width;
	int		win_height;
	t_map	*map;
	t_point	**screen_coords;
}	t_data;

/*** FUNCTION PROTOTYPES ***/
// Error handling
void		exit_error(char *msg);
void		free_split(char **arr);

// Parsing
void		parse_map(char *filename, t_map **map);
void		parse_line(char *line, int *z_row);

// Projection
void		compute_projection(t_data *data);
void		scale_and_center(t_data *data);

// Drawing
void		put_pixel(t_data *data, int x, int y, int color);
void		draw_line(t_point p0, t_point p1, t_data *data);
void		draw_map(t_data *data);

// Events
int			key_hook(int keycode, t_data *data);
int			close_hook(t_data *data);

#endif