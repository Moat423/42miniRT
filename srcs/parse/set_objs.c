
#include "../../include/miniRT.h"
#include "../../include/parse.h"

/*
* sp 0.0,0.0,20.6 12.6 10,0,255
* identifier: sp
∗ x, y, z coordinates of the sphere center: 0.0,0.0,20.6
∗ the sphere diameter: 12.6
∗ R,G,B colors in the range [0-255]: 10, 0, 255
* */
int	set_sphere(char *line, t_sphere *sphere)
{
	int		i;
	int		error;
	char	*number;
	int		previ;

	i = 3;
	error = 0;
	i = set_vec(line, i, &(sphere->pos));
	if (!i)
		return (0);
	i = ft_skip_space(line, i);
	previ = i;
	i = ft_float_len(&line[i]);
	number = ft_substr(line, previ, i);
	if (!number)
		return (ft_rperror("malloc)"));
	sphere->radius = ft_strtof(number, &error);
	if (!sphere->radius && error)
		return (ft_parseerror("invalid number", line));
	i = ft_skip_space(line, i);
	if (line[i] != '\n')
		i = set_color(line, i, &(sphere->color));
	return (1);
}

/*
* pl 0.0,0.0,-10.0 0.0,1.0,0.0 0,0,225
∗ identifier: pl
∗ x, y, z coordinates of a point in the plane: 0.0,0.0,-10.0
∗ 3D normalized normal vector, in the range [-1,1] for each x, y, z axis:
0.0,1.0,0.0
∗ R,G,B colors in the range [0-255]: 0,0,225
*/
int	set_plane(char *line, t_plane *plane)
{
	int			i;

	i = 3;
	i = set_vec(line, i, &(plane->pos));
	if (!i)
		return (0);
	i = ft_skip_space(line, i);
	i = set_vec(line, i, &(plane->normal));
	if (!i)
		return (0);
	i = ft_skip_space(line, i);
	if (line[i] != '\n')
		i = set_color(line, i, &(plane->color));
	return (1);
}

/*
* cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255
∗ identifier: cy
∗ x, y, z coordinates of the center of the cylinder: 50.0,0.0,20.6
∗ 3D normalized vector of axis of cylinder, in the range [-1,1] for each x, y,
z axis: 0.0,0.0,1.0
∗ the cylinder diameter: 14.2
∗ the cylinder height: 21.42
∗ R, G, B colors in the range [0,255]: 10, 0, 255
*/
int	set_cylinder(char *line, t_cylinder *cylinder)
{
	int			i;
	int			error;
	char		*number;
	int			previ;

	i = 3;
	error = 0;
	i = set_vec(line, i, &(cylinder->pos));
	if (!i)
		return (0);
	i = ft_skip_space(line, i);
	i = set_vec(line, i, &(cylinder->axis));
	if (!i)
		return (0);
	previ = ft_skip_space(line, i);
	i = ft_float_len(&line[i]);
	number = ft_substr(line, previ, i);
	if (!number)
		return (ft_rperror("malloc)"));
	cylinder->diameter = ft_strtof(number, &error);
	if (!cylinder->diameter && error)
		return (ft_parseerror("invalid number", line));
	previ = ft_skip_space(line, i);
	i = ft_float_len(&line[i]);
	number = ft_substr(line, i, ft_float_len(&line[i]));
	if (!number)
		return (ft_rperror("malloc)"));
	cylinder->height = ft_strtof(number, &error);
	i = ft_skip_space(line, i);
	if (!cylinder->height && error)
		return (ft_parseerror("invalid number", line));
	i = ft_skip_space(line, i);
	i = set_color(line, i, &(cylinder->color));
	if (!i)
		return (0);
	return (1);
}
