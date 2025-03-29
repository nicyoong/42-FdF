#include "minilibx-linux/mlx.h"

#define WIDTH 800
#define HEIGHT 600

typedef struct s_data {
    void    *mlx;
    void    *win;
    void    *img;
    char    *addr;
    int     bpp;
    int     line_length;
    int     endian;
}               t_data;

void    put_pixel(t_data *data, int x, int y, int color)
{
    char    *dst;

    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        dst = data->addr + (y * data->line_length + x * (data->bpp / 8));
        *(unsigned int*)dst = color;
    }
}

int     main(void)
{
    t_data  data;
    int     x;
    int     y;

    // Initialize MLX connection and window
    data.mlx = mlx_init();
    data.win = mlx_new_window(data.mlx, WIDTH, HEIGHT, "Image Test");
    
    // Create image buffer
    data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
    data.addr = mlx_get_data_addr(data.img, &data.bpp, &data.line_length, &data.endian);

    // Draw gradient pattern
    y = -1;
    while (++y < HEIGHT)
    {
        x = -1;
        while (++x < WIDTH)
        {
            // Create RGB color based on position
            int r = (x * 255) / WIDTH;
            int g = (y * 255) / HEIGHT;
            int b = 255 - ((x + y) * 255) / (WIDTH + HEIGHT);
            int color = (r << 16) | (g << 8) | b;
            
            put_pixel(&data, x, y, color);
        }
    }

    // Display image in window
    mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
    
    // Start event loop
    mlx_loop(data.mlx);
    return (0);
}