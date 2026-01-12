#include "minilibx-linux/mlx.h"
#include <stdlib.h>

int	main(void)
{
	void	*mlx;
	void	*win;
	int		i;

	mlx = mlx_init();
	win = mlx_new_window(mlx, 800, 600, "Test FDF");

	// Dessine une ligne rouge
	i = 100;
	while (i < 700)
	{
		mlx_pixel_put(mlx, win, i, 300, 0xFF0000);
		i++;
	}

	mlx_loop(mlx);
	return (0);
}
