/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:48:03 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/31 14:51:32 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"
#include "../../include/parse.h"

static int	parse_file(int fd, t_scene *scene);
static int	get_arrays(int fd, t_scene *scene);
static int	set_obj(char *line, t_scene *scene, int *indexi);
static void	count_obj(char *line, t_scene *scene);

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

static void	count_obj(char *line, t_scene *scene)
{
	if (!ft_strncmp(line, "L ", 2))
		scene->light_count++;
	else if (!ft_strncmp(line, "sp ", 3))
		scene->sphere_count++;
	else if (!ft_strncmp(line, "pl ", 3))
		scene->plane_count++;
	else if (!ft_strncmp(line, "cy ", 3))
		scene->cylinder_count++;
	else if (!ft_strncmp(line, "co ", 3))
		scene->cylinder_count++;
}

static int	parse_file(int fd, t_scene *scene)
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
		if (!ft_strncmp(line, "A ", 2))
			++got_ambient;
		else if (!ft_strncmp(line, "C ", 2))
			++got_camera;
		else
			count_obj(line, scene);
		free(line);
	}
	if (got_camera != 1 || got_ambient != 1)
		return (ft_parseerror("need 1 camera (C) & 1 ambient light (A)", NULL));
	get_next_line(-1);
	return (1);
}

// checks for object type and sets object (except camera and ambient)
static int	set_obj(char *line, t_scene *scene, int *indexi)
{
	if (!ft_strncmp(line, "L ", 2))
		return (set_light(line, &(scene->lights[indexi[LIGHT]++])));
	else if (!ft_strncmp(line, "sp ", 3))
		return (set_sphere(line, &(scene->spheres[indexi[SPHERE]++])));
	else if (!ft_strncmp(line, "pl ", 3))
		return (set_plane(line, &(scene->planes[indexi[PLANE]++])));
	else if (!ft_strncmp(line, "cy ", 3))
		return (set_cylinder(line, &(scene->cylinders[indexi[CYLINDER]++])));
	else if (!ft_strncmp(line, "co ", 3))
		return (set_cone(line, &(scene->cones[indexi[CONE]++])));
	else if (!ft_strncmp(line, "A ", 2))
		return (set_ambient(line, &(scene->ambient)));
	else if (!ft_strncmp(line, "C ", 2))
		return (set_camera(line, &(scene->camera)));
	return (ft_parseerror("invalid object", line));
}

static int	get_arrays(int fd, t_scene *scene)
{
	char		*line;
	int			indexi[OBJ_NUM];

	ft_bzero(indexi, sizeof(int) * OBJ_NUM);
	if (!ft_malloc_scene_arrays(scene))
	{
		scene_destroy(scene);
		return (0);
	}
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
