#include "../../include/miniRT.h"
#include "../../include/parse.h"

int	set_light(char *line, t_light *light)
{
	int			i;

	i = 2;
	i = set_vec(line, i, &(light->pos));
	if (!i)
		return (0);
	i = ft_skip_space(line, i);
	light->brightness = ft_strtod_s(&line[i], NULL);
	if (!light->brightness && errno)
		return (ft_parseerror("invalid number", line));
	i = ft_skip_space(line, i);
	if (line[i] != '\n')
		i = set_color(line, i, &(light->color));
	return (1);
}

int	set_ambient(char *line, t_ambient *ambient)
{
	int			i;

	i = 2;
	ambient->brightness = ft_strtod_s(&line[i], NULL);
	if (!ambient->brightness && errno)
		return (ft_parseerror("invalid number", line));
	i = ft_skip_space(line, i);
	i = set_color(line, i, &(ambient->color));
	if (!i)
		return (0);
	return (1);
}

int	set_camera(char *line, t_camera *camera)
{
	int			i;

	i = 2;
	i = set_vec(line, i, &(camera->pos));
	if (!i)
		return (0);
	i = ft_skip_space(line, i);
	i = set_vec(line, i, &(camera->dir));
	if (!i)
		return (0);
	i = ft_skip_space(line, i);
	camera->fov = ft_strtoimax(&line[i], NULL, 10);
	if (!(camera->fov) && errno)
		return (ft_parseerror("invalid number", line));
	camera->up = vec3_new(0, 1, 0);
	camera->right = vec3_cross(camera->dir, camera->up);
	return (1);
}
