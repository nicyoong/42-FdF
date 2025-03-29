# Name of the executable
NAME    := fdf

# List your source C files here (add or remove as needed)
SRCS    := \
			fdf.c \
			hook.c \
			draw.c \
			fdfutils.c \
			fdfparser.c \
			main.c
OBJS    := $(SRCS:.c=.o)

# Compiler and flags
CC      := cc
CFLAGS  := -Wall -Wextra -Werror

# Directories for the libraries
LIBFT_DIR    := ./libft
MLX_DIR      := ./minilibx-linux

# Library filenames (to be copied into the current directory)
LIBFT_A      := libft.a
MLX_LINUX_A  := libmlx_Linux.a
MLX_A        := libmlx.a

# Default rule: build the executable
all: $(NAME)

# Build libft and copy its library if needed
$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)
	$(MAKE) bonus -C $(LIBFT_DIR)
	cp -u $(LIBFT_DIR)/$(LIBFT_A) .

# Build minilibx libraries (both) and copy them if needed.
$(MLX_LINUX_A) $(MLX_A):
	$(MAKE) -C $(MLX_DIR) -f Makefile.mk
	cp -u $(MLX_DIR)/$(MLX_LINUX_A) .
	cp -u $(MLX_DIR)/$(MLX_A) .

# Link the final executable.
# It depends on the object files and the three library archives.
$(NAME): $(OBJS) $(LIBFT_A) $(MLX_LINUX_A) $(MLX_A)
	$(CC) $(OBJS) $(MLX_LINUX_A) $(LIBFT_A) $(MLX_A) -lX11 -lXext -lm $(CFLAGS) -o $(NAME)

# Compile .c files to .o object files.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean only object files and call clean in the subdirectories.
clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(MLX_DIR) -f Makefile.mk clean

# fclean removes objects, the executable, and the library files.
fclean: clean
	rm -f $(NAME) $(LIBFT_A) $(MLX_LINUX_A) $(MLX_A)
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(MLX_DIR) -f Makefile.mk clean

# re: fclean then all
re: fclean all

# Declare non-file targets
.PHONY: all clean fclean re
