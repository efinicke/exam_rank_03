# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

# define ARG_ERR "Error: argument\n"
# define COR_ERR "Error: Operation file corrupted\n"

FILE 	*file;
char 	**map = NULL;

int	width;
int	height;
char	background;

char	type;
float	x_corner;
float	y_corner;
float	r_width;
float	r_height;
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
	if (width <= 0 || height <= 0)
		return (0); 
	if (width > 300 || height > 300)
		return (0);
	return (1);
}

int	is_foreground_OK(void)
{
	if (type != 'r' && type != 'R')
		return (0);
	if (r_height <= 0 || r_width <= 0)
		return (0);
	return (1);
}

int     fill_background(void)
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

int	calcul(float x, float y)
{
	if ((((x < x_corner) || (x_corner + r_width < x)) || (y < y_corner)) || (y_corner + r_height < y))
		return (0);
	if (((x - x_corner < 1.00000000) || ((x_corner + r_width) - x < 1.00000000)) ||
			((y - y_corner < 1.00000000 || ((y_corner + r_height) - y < 1.00000000))))
		return (2); // Border
	return (1);		// Inside
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
			if ((calcul((float)x, (float)y) == 2) || ((calcul((float)x, (float)y) == 1 && type == 'R')))
				map[y][x] = foreground;
			x++;
		}
		y++;
	}
	return (1);
}

int	fill_map(void)
{
	int ret;

	ret = fscanf(file, "%d %d %c\n",\
			&width, &height, &background);
	if (ret != 3)
		return (0);
	if (!is_background_OK())
		return (0);
	fill_background();
	ret = 0;
	ret = fscanf(file, "%c %f %f %f %f %c\n",\
			&type, &x_corner, &y_corner, &r_width, &r_height, &foreground);
	while (ret == 6)
	{
		if (!is_foreground_OK())
			return (0);
		fill_foreground();
		ret = fscanf(file, "%c %f %f %f %f %c\n",\
				&type, &x_corner, &y_corner, &r_width, &r_height, &foreground);
	}
	if (ret != -1)
		return (0);
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
