#include "minilibx-linux/mlx.h"

int main()
{
    void *mlx;
    void *win;

    mlx = mlx_init();  // Initialize MLX
    win = mlx_new_window(mlx, 800, 600, "My Window");  // Create a window

    mlx_loop(mlx);  // Keep the window open
    return (0);
}

//compile command cc mlxtest.c minilibx-linux/libmlx_Linux.a minilibx-linux/libmlx.a -lX11 -lXext -lm