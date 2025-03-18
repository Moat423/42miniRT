#include "../../include/miniRT.h"

int	parse_file(int fd, t_scene *scene);
int	ft_parseerror(char *error, char *line);
int	rperror(char *str);
t_camera	set_camera(char *line);


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
	char	*line;
	char	ident[3];
	int		readlen;
	int		linebreak;
	int		got_camera;

	ft_bzero(ident, 3);
	readlen = 1;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '\n')
			continue ;
		if (line[0] == 'C' && line[1] == ' ')
			scene->camera = set_camera(line);
	}

	// linebreak = 1;
	// while (ft_strchr(ident, '\n'))
	// 	readlen = read(fd, ident, 1);
	// while (readlen > 0)
	// {
	// 	while (ft_strchr(ident, '\n'))
	// 	{
	// 		readlen = read(fd, ident, 1);
	// 		linebreak = 1;
	// 	}
	// 	readlen = read(fd, ident, 2);
	// }
	get_next_line(-1);
}

//
t_camera	set_camera(char *line)
{
	int		i;
	char	*num;
	int		error;
	
	i = 1;
	while (line[++i] != '\n') 
	{
		if (line[i] == ' ')
			continue;
		if (ft_isdigit(line[i]) || line[i] == '-')
		{
			num = ft_substr(line, i, line - ft_strchr(&line[i], ','));
			ft_strtod_s(num, &error);
		}
	}
}

int	set_pos(char *line, int start, t_vec3 *pos)
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
		comma = line - ft_strchr(&line[start], ',');
		num = ft_substr(line, start, comma );
		if (!num)
			rperror("malloc");
		arr[i] = ft_strtod_s(num, &error);
		free(num);
		if (error)
			return (ft_parseerror("number to large in line:", line));
		start = comma + 1;
		++i;
	}
	*pos = vec3_new(arr[0], arr[1], arr[2]);
	return (0);
}

int	ft_parseerror(char *error, char *line)
{
	ft_fprintf(2, "ERROR\n%s\n    %s", error, line);
	return (1);
}

int	rperror(char *str)
{
	perror(str);
	return (errno);
}
