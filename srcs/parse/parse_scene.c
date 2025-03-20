#include "../../include/miniRT.h"
#include "../../include/parse.h"

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

// return 0 on error, -1 if no camera, 1 if camera
int	get_camera(char *line, t_camera *camera, int *got_camera)
{
	if (line[0] == 'C' && line[1] == ' ')
	{
		if (!set_camera(line, camera))
			return (0);
		++(*got_camera);
		return (1);
	}
	return (-1);
}

// return 0 on error, -1 if no ambient, 1 if ambient
int	get_ambient(char *line, t_ambient *ambient, int *got_ambient)
{
	if (line[0] == 'C' && line[1] == ' ')
	{
		if (!set_ambient(line, ambient))
			return (0);
		++(*got_ambient);
		return (1);
	}
	return (-1);
}

int	parse_file(int fd, t_scene *scene)
{
	char	*line;
	int		got_camera;
	int		got_ambient;

	got_camera = 0;
	got_ambient = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '\n')
			continue ;
		if (!get_camera(line, &(scene->camera), &got_camera))
			return (0);
		else if (!get_ambient(line, &(scene->ambient), &got_ambient))
			return (0);
		else if (!ft_strncmp(line, "L ", 2))
			scene->light_count++;
		else if (!ft_strncmp(line, "sp ", 3))
			scene->sphere_count++;
		else if (!ft_strncmp(line, "pl ", 3))
			scene->plane_count++;
		else if (!ft_strncmp(line, "cy ", 3))
			scene->cylinder_count++;
		free(line);
	}
	printf("got_camera: %d\n", got_camera);
	printf("got_ambient: %d\n", got_ambient);
	if (got_camera != 1 || got_ambient != 1)
		return (ft_parseerror("need 1 camera (C) & 1 ambient light (A)", NULL));
	get_next_line(-1);
	return (1);
}

// checks for object type and sets object (except camera and ambient)
int	set_obj(char *line, t_scene *scene, int *indexi)
{
	if (!ft_strncmp(line, "L ", 2))
		return (set_light(line, &(scene->lights[indexi[LIGHT]++])));
	else if (!ft_strncmp(line, "sp ", 3))
		return (set_sphere(line, &(scene->spheres[indexi[SPHERE]++])));
	else if (!ft_strncmp(line, "pl ", 3))
		return (set_plane(line, &(scene->planes[indexi[PLANE]++])));
	else if (!ft_strncmp(line, "cy ", 3))
		return (set_cylinder(line, &(scene->cylinders[indexi[CYLINDER]++])));
	else if (!ft_strncmp(line, "A ", 2) || !ft_strncmp(line, "C ", 2))
		return (1);
	return (ft_parseerror("invalid object", line));
}

int	get_arrays(int fd, t_scene *scene)
{
	char		*line;
	int			indexi[4];

	ft_bzero(indexi, sizeof(int) * 4);
	if (!ft_malloc_scene_arrays(scene))
		return (0);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '\n')
			continue ;
		if (!set_obj(line, scene, indexi))
			return (0);
		free(line);
	}
	get_next_line(-1);
	return (1);
}
