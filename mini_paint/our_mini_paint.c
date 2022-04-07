# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <math.h>

# define ARG_ERR "Error: argument\n"
# define COR_ERR "Error: Operation file corrupted\n"


char 	**map;
FILE	*file;

int	width;
int	height;
char	background;

char	type;
float	x_center; // position horizontale au centre du cercle.
float	y_center; // position verticale au centre du cercle.
float	radius; // rayon du cercle qui doit etre positif.
char	foreground;

int	ft_strlen(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void	free_map()
{
	int i;

	i = 0;
	if (map)
	{
		while (i < height)
		{
			free(map[i]);
			i++;
		}
		free(map);
	}
}

int	send_error(char *str)
{
	write(1, str, ft_strlen(str));
	free_map();
	return (1);
}

int	is_background_OK(void)
{
	if (width <= 0 || height <= 0 
			|| width > 300 || height > 300)
		return (0);
	return (1);
}

int	fill_background(void)
{
	int y;

	y = 0;
	map = calloc(height, sizeof(char *));
	if (!map)
		return (0);
	while (y < height)
	{
		map[y] = calloc(width + 1, 1);
		memset(map[y], background, width);
		y++;
	}
	return (1);
}

int	is_foreground_OK(void)
{
	if (type != 'c' && type != 'C')
		return (0);
	if (radius <= 0.00000000)
		return (0);
	return (1);
}

int	calcul(int x, int y)
{
	float	dist = 0;
	float	distance;

	dist = sqrtf(((x - x_center) * (x - x_center) + (y - y_center) * (y - y_center)));
	distance = dist - radius;
	if (distance <= 0.00000000)
	{
		if (distance <= -1.00000000)
			return (1);
		return (2);
	}
	return (0);
}

int	fill_foreground(void)
{
	int x;
	int y;

	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{	
			if (calcul(x, y) == 2 || (calcul(x, y) == 1 && type == 'C'))
				map[y][x] = foreground;
			x++;
		}
		y++;
	}
	return (1);
}

void	draw_map(void)
{
	int i;

	i = 0;
	while (i < height)
	{
		write(1, map[i], ft_strlen(map[i]));
		write(1, "\n", 1);
		i++;
	}
	free_map();
}

int	fill_map()
{
	int ret;

	ret = fscanf(file, "%d %d %c\n", &width, &height, &background);
	if (ret != 3)
		return (0);
	if (!is_background_OK())
		return (0);
	fill_background();
	ret = fscanf(file, "%c %f %f %f %c\n", &type, &x_center, &y_center, &radius, &foreground);
	while (ret == 5)
	{
		if (!is_foreground_OK())
			return (0);
		fill_foreground();
		ret = fscanf(file, "%c %f %f %f %c\n", &type, &x_center, &y_center, &radius, &foreground);
	}
	if (ret != -1)
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (send_error(ARG_ERR));
	if (!(file = fopen(argv[1], "r")))
		return (send_error(COR_ERR));
	if (!fill_map())
		return (send_error(COR_ERR));
	draw_map();
	if (file)
		fclose(file);
	return (0);
}
