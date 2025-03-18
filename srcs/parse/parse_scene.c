#include "../../include/miniRT.h"

int	parse_file(int fd, t_scene *scene);
int	set_camera(char *line, t_camera *camera);
int	set_vec(char *line, int start, t_vec3 *vec);
int	ft_parseerror(char *error, char *line);
int	rperror(char *str);


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
	parse_file(fd, scene);
	return (0);
}

int	parse_file(int fd, t_scene *scene)
{
	char		*line;
	bool		got_camera;
	bool		got_ambient_light;
	bool		got_light;

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
		// if (line[0] == 'A' && line[1] == ' ')
		// {
		// 	if (!set_camera(line, &(scene->camera)))
		// 		return (0);
		// 	got_ambient_light = true;
		// }
		// if (line[0] == 'L' && line[1] == ' ')
		// {
		// 	if (!set_camera(line, &(scene->camera)))
		// 		return (0);
		// 	got_light = true;
		// }
	}
	get_next_line(-1);
	return (1);
}


int	set_camera(char *line, t_camera *camera)
{
	int			i;

	i = 2;
	i = set_vec(line, i, &(camera->pos));
	if (!i)
		return (0);
	while (line[i] == ' ')
		++i;
	i = set_vec(line, i, &(camera->dir));
	if (!i)
		return (0);
	while (line[i] == ' ')
		++i;
	camera->fov = ft_strtoimax(&line[i], NULL, 10);
	if (!(camera->fov) && errno)
		return (ft_parseerror("number contains invalid characters or too large", line));
	camera->up = vec3_new(0, 1, 0);
	camera->right = vec3_cross(camera->dir, camera->up);
	return (1);
}

int	set_vec(char *line, int start, t_vec3 *vec)
{
	int		i;
	char	*num;
	int		error;
	int		arr[3];
	int		comma;

	i = 0;
	error = 0;
	while (i < 3)
	{
		if (i < 2)
			comma = line - ft_strchr(&line[start], ',');
		else
			comma = line - ft_strchr(&line[start], ' ');
		num = ft_substr(line, start, comma);
		if (!num)
			return (0);
		arr[i] = ft_strtod_s(num, &error);
		free(num);
		if (error)
			return (ft_parseerror("number contains invalid characters or too large", line));
		start = comma + 1;
		++i;
	}
	*vec = vec3_new(arr[0], arr[1], arr[2]);
	return (comma + 1);
}

// returns index of first invalid character
int	ft_float_len(char *str)
{
	int	dotcount;
	int	i;

	dotcount = 0;
	i = ft_skip_whitespace(str);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] == '.' )
		{
			if (dotcount == 0)
				dotcount++;
			else
				return (i);
		}
		else if (str[i] < '0' || str[i] > '9')
			return (i);
		i++;
	}
	return (0);
}

int	ft_parseerror(char *error, char *line)
{
	ft_fprintf(2, "ERROR\n%s\n   line:\n%s", error, line);
	return (0);
}

int	rperror(char *str)
{
	perror(str);
	return (errno);
}
