/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_objs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:07:49 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/16 16:58:17 by moat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"
#include "../../include/parse.h"

static t_vec3	move_point(t_vec3 point, t_vec3 dir, float dist)
{
	return (vec3_add(point, vec3_multiply(dir, dist)));
}

/*
 * co 0.0,4.0,2.0 0.0,0.0,1.0 14.2 21.42 10,0,255
 *  |	|			|			|	|		|
 * identifier: co	|			|	|		|
 * 		|			|			|	|		|
 * x, y, z coordinates of the top point of the cone
 * 					|			|	|		|
∗ 3D normalized vector of axis of cone, in range [-1,1] for each x, y, z axis
								|	|		|
∗ 				  the cone diameter	|		|
∗ 						the cone height		|
∗					R, G, B colors in the range [0,255]
*/
int	set_cone(char *line, t_cone *cone)
{
	int			i;

	i = 3;
	i = set_vec(line, i, &(cone->top));
	if (!i || line[i - 1] != ' ')
		return (0);
	i = ft_skip_space(line, i);
	i = set_vec(line, i, &(cone->axis));
	if (!i || line[i - 1] != ' ')
		return (0);
	i = ft_substrtof(&(cone->radius), i, line);
	if (!i || line[i - 1] != ' ')
		return (0);
	cone->radius /= 2;
	i = ft_substrtof(&(cone->height), i, line);
	if (!i || line[i - 1] != ' ')
		return (0);
	i = set_color(line, i, &(cone->color));
	if (!i || line[i - 1] != '\n')
		return (0);
	cone->bottom = move_point(cone->top, cone->axis, cone->height);
	cone->slant = cone->radius / cone->height;
	return (1);
}

/*
* sp 0.0,0.0,20.6 12.6 10,0,255 ch
* identifier: sp
∗ x, y, z coordinates of the sphere center: 0.0,0.0,20.6
∗ the sphere diameter: 12.6
∗ R,G,B colors in the range [0-255]: 10, 0, 255
* optional: ch to apply checkers texture
*/
int	set_sphere(char *line, t_sphere *sphere)
{
	int		i;

	i = 3;
	i = set_vec(line, i, &(sphere->pos));
	if (!i || line[i - 1] != ' ')
		return (0);
	i = ft_skip_space(line, i);
	i = ft_substrtof(&(sphere->radius), i, line);
	if (!i || !(line[i - 1] == ' ' || line[i - 1] == '\n'))
		return (0);
	sphere->radius /= 2;
	i = set_color(line, i, &(sphere->color));
	if (!i)
		return (0);
	if (set_texturing(&(line[i]), &(sphere->texturing)))
		return (1);
	ft_fprintf(2, "failed settting texture\n");
	ft_fprintf(2, "expected texture %d, incorrect values were given\n", sphere->texturing.type);
	// sphere->texturing.type = ft_set_texture_flag(&i, line);
	// if (sphere->texturing.type == BUMP)
	// 	return (set_bumpmap(&(line[i]), &(sphere->bumpmap)));
	// if (line[i - 1] == '\n')
	// 	return (1);
	// ft_fprintf(2, "Error\nwrong line ending line:\n%s", line);
	return (0);
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
	if (!i || line[i - 1] != ' ')
		return (0);
	i = ft_skip_space(line, i);
	i = set_vec(line, i, &(plane->normal));
	if (!i || line[i - 1] != ' ')
		return (0);
	i = ft_skip_space(line, i);
	if (line[i] != '\n')
		i = set_color(line, i, &(plane->color));
	if (!i)
		return (0);
	plane->texture = ft_set_texture_flag(&i, line);
	if (line[i - 1] != '\n')
		return (0);
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
* sets top as the end point where the cylinders normal points to
* sets bottom as the end point where the normal points inside the cylinder
*/
int	set_cylinder(char *line, t_cylinder *cyl)
{
	int			i;

	i = 3;
	i = set_vec(line, i, &(cyl->pos));
	if (!i || line[i - 1] != ' ')
		return (0);
	i = ft_skip_space(line, i);
	i = set_vec(line, i, &(cyl->axis));
	if (!i || line[i - 1] != ' ')
		return (0);
	i = ft_substrtof(&(cyl->radius), i, line);
	if (!i || line[i - 1] != ' ')
		return (0);
	cyl->radius /= 2;
	i = ft_substrtof(&(cyl->height), i, line);
	if (!i || line[i - 1] != ' ')
		return (0);
	i = set_color(line, i, &(cyl->color));
	if (!i || line[i - 1] != '\n')
		return (0);
	cyl->top = move_point(cyl->pos, cyl->axis, cyl->height / 2);
	cyl->bottom = move_point(cyl->pos, cyl->axis, -cyl->height / 2);
	return (1);
}
