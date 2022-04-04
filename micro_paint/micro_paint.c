# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

# define ARG_ERR "Error: argument\n"
# define COR_ERR "Error: operation file corrupted\n"

FILE 	*file;
char 	**map;

int		width;
int		height;
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

int	send_error(char *str)
{
	write(1, str, ft_strlen(str));
	return (1);
}

int	is_background_OK(void)
{
	if (width < 0 || height < 0 
		|| width >= 300 || height >= 300)
		return (0);
	return (1);
}

int	is_foreground_OK(void)
{
	if (type != 'r' && type != 'R')
		return (0);
	if (x_corner < 0 || y_corner < 0)
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

//width height background
// 20     20      ~

// type x_corner y_corner r_width r_height foreground;
//  r     10        10       3       3        O

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
			if (type == 'R')
			{
				if ( (x >= x_corner && x <= x_corner + r_width) && (y >= y_corner && y <= y_corner + r_height))
					map[y][x] = foreground;
			}
			if (type == 'r')
			{
				if ( (x == x_corner) && (y >= y_corner && y <= y_corner + r_height))
					map[y][x] = foreground;
				if ( (y == y_corner) && (x >= x_corner && x <= x_corner + r_width))
					map[y][x] = foreground;
				if ( (x == x_corner + r_width) && (y >= y_corner && y <= y_corner + r_height))
					map[y][x] = foreground;
				if ( (y == y_corner + r_height) && (x >= x_corner && x <= x_corner + r_width))
					map[y][x] = foreground;
			}
			x++;
		}
		y++;
	}
	return (1);
}

int	fill_map(void)
{
	int ret;

	ret = 0;
	ret = fscanf(file, "%d %d %c\n", &width, &height, &background);
	if (ret != 3 || ret == -1)
		return (0);
	if (!is_background_OK())
		return (0);
	fill_background();
	ret = 0;
	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &type, &x_corner, &y_corner, &r_width, &r_height, &foreground) != -1))
	{
		if (!is_foreground_OK())
			return (0);
		fill_foreground();
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
	return (0);
}
