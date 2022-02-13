#include "mb.h"
# define WIDTH			1920
# define HEIGHT			1080

double colorrange = 1;
int morecolors = 0;
double globalx = 0;
double globaly = 0;
double step = 0;
double	zoom = 1;
int	iter = 100;
int	abstrue = 0;

typedef struct	s_data {
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	void	*mlx;
	void	*win;
	void	*img;
}				t_data;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int	fillcolor(double cr, double ci)
{
	int	count = 1;
	double	zr = 0;
	double	tempzr = 0;
	double	zi = 0;
	while (count != iter)
	{
		if (abstrue)
		{
			zr = fabs(zr);
			zi = fabs(zi);
		}
		tempzr = zr;
		zr = (zr * zr) - (zi * zi);
		zi = (tempzr * zi) + (tempzr * zi);
		zr += cr;
		zi += ci;
		if (zr > 4.00 || zr < -4.00 || zi > 4.00 || zi < -4.00)
		{
			double itert;
			itert = (double)iter;
			if (morecolors)
				itert /= zr * zi;
			return ((((float)count) / (float)itert * (float)colorrange) * (0xFFFFFF));
		}
		count++;
	}
	return (0x000000);
}

void	dopixels(t_data *fdf)
{
	double	size_y = 3.3;
	double	size_x = size_y * ((double)WIDTH/(double)HEIGHT);
	double	x = 0;
	double	y = 0;
	int color;
	while (y != HEIGHT)
	{
		if (x == WIDTH)
			y++, x = 0;
		color = fillcolor(zoom * (size_x/WIDTH*x - size_x / 2.0) + globalx,
				zoom * (size_y/HEIGHT*y - size_y / 2.0) + globaly);
		my_mlx_pixel_put(fdf, x, y, color);
		x++;
	}
}

int	dostuff(int key, void *param)
{
	t_data *fdf;
	fdf = (t_data *)param;
	if (key == 0xff53) //oikea
		globalx += 0.2 * zoom;
	else if (key == 0xff52)
		globaly -= 0.2 * zoom;
	else if (key == 0xff51) //vasen
		globalx -= 0.2 * zoom;
	else if (key == 0xff54)
		globaly += 0.2 * zoom;
	else if (key == 0xffad)
		iter /= 2;
	else if (key == 0xffab)
		iter *= 2;
	else if (key == 0xff55)
		zoom /= 2;
	else if (key == 0xff56)
		zoom *= 2;
	else if (key == 'q')
		colorrange *= 2;
	else if (key == 'w')
		colorrange /= 2;
	else if (key == 'r') //reset
	{
		colorrange = 1;
		morecolors = 0;
		globalx = 0;
		globaly = 0;
		step = 0;
		zoom = 1;
		iter = 100;
		abstrue = 0;
	}
	else if (key == 's')
	{
		if (!morecolors)
			morecolors = 1;
		else
			morecolors = 0;
	}
	else if (key == 0x20)
	{
		if (!abstrue)
			abstrue = 1;
		else
			abstrue = 0;
	}
	else
		return (0);
	dopixels(fdf);
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->img, 0, 0);
	return (1);
}

int main()
{
	t_data fdf;

	fdf.mlx = mlx_init();
	fdf.win = mlx_new_window(fdf.mlx, WIDTH, HEIGHT, "FdF");
	fdf.img = mlx_new_image(fdf.mlx, WIDTH, HEIGHT);
	fdf.addr = mlx_get_data_addr(fdf.img, &fdf.bits_per_pixel, &fdf.line_length, &fdf.endian);
	dopixels(&fdf);
	mlx_put_image_to_window(fdf.mlx, fdf.win, fdf.img, 0, 0);
	mlx_key_hook(fdf.win, dostuff, &fdf);
	mlx_loop(fdf.mlx);
}

