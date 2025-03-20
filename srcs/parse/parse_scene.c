#include "../../include/miniRT.h"
#include "../../include/parse.h"

static float	ft_after_point(char *str)
{
	float	pos;
	float	res_b;

	pos = 1;
	res_b = 0;
	while (*str <= '9' && *str >= '0')
	{
		pos /= 10;
		res_b += pos * (*str - '0');
		str++;
	}
	return (res_b);
}

float	ft_atof(char *str)
{
	float	res_a;
	int		sign;
	float	res_b;

	sign = 1;
	while ((*str >= '\t' && *str <= '\r') || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
		if (*(str++) == '-')
			sign = -1;
	res_a = 0;
	while (*str >= '0' && *str <= '9')
		res_a = res_a * 10 + *(str++) - '0';
	if (*(str++) != '.')
		return (res_a * sign);
	res_b = ft_after_point(str);
	return ((res_a + res_b) * sign);
}

//returns attempted ft_atof, and sets error to 1 if not floatable, else error 0
float	ft_strtof(char *str, int *error)
{
	*error = 0;
	if (ft_isdoubleable(str))
	{
		*error = 1;
		return (0);
	}
	return (ft_atof(str));
}

int	parse_scene(char *filename, t_scene *scene)
{
	int	fd;

	if (!filename || filename[0] == 0)
	{
		ft_fprintf(2, "invalid empty filename");
		return (1);
	}
	ft_bzero(scene, sizeof(t_scene));
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_fprintf(2, "invalid file: %s", strerror(errno));
		return (1);
	}
	if (!parse_file(fd, scene))
		return (1);
	close(fd);
	fd = open(filename, O_RDONLY);
	if (!get_arrays(fd, scene))
		return (1);
	return (0);
}

// return -1 on error, 0 if no camera, 1 if camera
int	get_camera(char *line, t_camera *camera, bool got_camera)
{
	if (line[0] == 'C' && line[1] == ' ')
	{
		if (!set_camera(line, camera))
			return (-1);
		got_camera = true;
		return (1);
	}
	return (0);
}

int	parse_file(int fd, t_scene *scene)
{
	char		*line;
	bool		got_camera;
	bool		got_ambient;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '\n')
			continue ;
		if (line[0] == 'C' && line[1] == ' ')
		{
			if (!set_camera(line, &(scene->camera)))
				return (0);
			got_camera = true;
		}
		else if (line[0] == 'A' && line[1] == ' ')
		{
			if (!set_ambient(line, &(scene->ambient)))
				return (0);
			got_ambient = true;
		}
		else if (ft_strncmp(line, "L ", 2))
			scene->light_count++;
		else if (ft_strncmp(line, "sp ", 3))
			scene->sphere_count++;
		else if (ft_strncmp(line, "pl ", 3))
			scene->plane_count++;
		else if (ft_strncmp(line, "cy ", 3))
			scene->cylinder_count++;
		free(line);
	}
	get_next_line(-1);
	return (1);
}

int	get_arrays(int fd, t_scene *scene)
{
	char		*line;
	int			light_index;
	int			sphere_index;
	int			plane_index;
	int			cylinder_index;

	light_index = 0;
	sphere_index = 0;
	plane_index = 0;
	cylinder_index = 0;
	if (ft_malloc_scene_arrays(scene))
		return (0);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '\n')
			continue ;
		if (line[0] == 'L' && line[1] == ' ')
		{
			if (!set_light(line, &(scene->lights[++light_index])))
			{
				free(line);
				return (0);
			}
		}
		else if (ft_strncmp(line, "sp ", 3))
			sphere_index++;
		else if (ft_strncmp(line, "pl ", 3))
			plane_index++;
		else if (ft_strncmp(line, "cy ", 3))
			cylinder_index++;
		free(line);
	}
	get_next_line(-1);
	return (1);
}
