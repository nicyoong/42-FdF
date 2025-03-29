#include "minilibx-linux/mlx.h"
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 400
#define HEIGHT 300

typedef struct s_data {
    void    *mlx;
    void    *win;
    int     mouse_x;
    int     mouse_y;
}               t_data;

int key_handler(int keycode, t_data *data)
{
    if (keycode == 53) // ESC key
    {
        mlx_destroy_window(data->mlx, data->win);
        exit(0);
    }
    printf("Key pressed: %d\n", keycode);
    return (0);
}

int mouse_handler(int button, int x, int y, t_data *data)
{
    data->mouse_x = x;
    data->mouse_y = y;
    printf("Mouse click: button %d at (%d, %d)\n", button, x, y);
    return (0);
}

int close_handler(t_data *data)
{
    printf("Window closed\n");
    mlx_destroy_window(data->mlx, data->win);
    exit(0);
}

int main(void)
{
    t_data  data;

    data.mlx = mlx_init();
    data.win = mlx_new_window(data.mlx, WIDTH, HEIGHT, "Event Test");

    // Set up event hooks
    mlx_hook(data.win, 2, 1L<<0, key_handler, &data);        // Key press
    mlx_hook(data.win, 4, 1L<<2, mouse_handler, &data);     // Mouse click
    mlx_hook(data.win, 17, 0, close_handler, &data);        // Window close

    // Start event loop
    mlx_loop(data.mlx);
    return (0);
}