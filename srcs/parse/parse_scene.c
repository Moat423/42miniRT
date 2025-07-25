/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:48:03 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/16 19:02:44 by moat             ###   ########.fr       */
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
		ft_fprintf(2, "invalid empty filename\n");
		return (1);
	}
	ft_bzero(scene, sizeof(t_scene));
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_fprintf(2, "invalid file: %s\n", strerror(errno));
		return (1);
	}
	if (!parse_file(fd, scene))
	{
		close(fd);
		return (1);
	}
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
		scene->objs.sphere_count++;
	else if (!ft_strncmp(line, "pl ", 3))
		scene->objs.plane_count++;
	else if (line[0] == 'c')
	{
		if (line[1] == 'y' && line[2] == ' ')
			scene->objs.cylinder_count++;
		else if (line[1] == 'o' && line[2] == ' ')
			scene->objs.cone_count++;
	}
}

static int	parse_file(int fd, t_scene *scene)
{
	char	*line;
	int		got_camera;
	int		got_ambient;

	got_camera = 0;
	got_ambient = 0;
	get_next_line(-1);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '\n')
			;
		else if (!ft_strncmp(line, "A ", 2))
			++got_ambient;
		else if (!ft_strncmp(line, "C ", 2))
			++got_camera;
		else
			count_obj(line, scene);
		free(line);
	}
	get_next_line(-1);
	return (check_count(scene, got_camera, got_ambient));
}

// checks for object type and sets object (except camera and ambient)
static int	set_obj(char *line, t_scene *scene, int *arrray_of_i)
{
	if (!ft_strncmp(line, "L ", 2))
		return (set_light(line, &(scene->lights[arrray_of_i[LIGHT]++])));
	else if (!ft_strncmp(line, "sp ", 3))
		return (set_sphere(line,
				&(scene->objs.spheres[arrray_of_i[SPHERE]++])));
	else if (!ft_strncmp(line, "pl ", 3))
		return (set_plane(line, &(scene->objs.planes[arrray_of_i[PLANE]++])));
	else if (!ft_strncmp(line, "cy ", 3))
		return (set_cylinder(line,
				&(scene->objs.cylinders[arrray_of_i[CYLINDER]++])));
	else if (!ft_strncmp(line, "co ", 3))
		return (set_cone(line, &(scene->objs.cones[arrray_of_i[CONE]++])));
	else if (!ft_strncmp(line, "A ", 2))
		return (set_ambient(line, &(scene->ambient)));
	else if (!ft_strncmp(line, "C ", 2))
		return (set_camera(line, &(scene->camera)));
	return (ft_parseerror("invalid object", line, 0));
}

static int	get_arrays(int fd, t_scene *scene)
{
	char		*line;
	int			indexi[OBJ_NUM];

	ft_bzero(indexi, sizeof(int) * OBJ_NUM);
	if (!ft_malloc_scene_arrays(scene))
		return (0);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '\n')
			;
		else if (!set_obj(line, scene, indexi))
		{
			get_next_line(-1);
			free(line);
			return (0);
		}
		free(line);
	}
	get_next_line(-1);
	return (1);
}
