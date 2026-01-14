#include "./includes/fdf.h"

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

/*int	main(int ac, char **av)
{
	int	fd;
	t_fdf data;

	if (ac != 2)
		return (ERROR);
	fd = open(av[1], O_RDONLY);
	data = parse_data(av[1]);
	ft_printf("couleur %d\nx %d\ny %d\nz %d\nheight : %d\nwidth : %d", data.map[2][2].color, data.map[2][2].x, data.map[2][2].y, data.map[2][2].z, data.height, data.width);
	return (0);
}*/
