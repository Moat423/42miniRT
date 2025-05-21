/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:51:50 by kwurster          #+#    #+#             */
/*   Updated: 2025/05/21 15:12:39 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color	sphere_color_at(t_sphere *sphere, t_vec3 pt)
{
	if (sphere->texturing.type == CHECKERS)
		return (sphere_pattern_at(
				vec3_subtract(pt, sphere->pos),
				sphere->radius, sphere->texturing.checkers));
	else if (sphere->texturing.type == TEXTURE)
		return (sphere_texture_at(
				vec3_subtract(pt, sphere->pos),
				sphere->radius, sphere->texturing.texture));
	return (sphere->color);
}

t_color	plane_color_at(t_plane *plane, t_vec3 pt)
{
	if (plane->texturing.type == CHECKERS)
		return (planar_pattern_at(plane->normal, pt,
				plane->texturing.checkers));
	else if (plane->texturing.type == TEXTURE)
		return (planar_texture_at(plane->normal,
				vec3_subtract(pt, plane->pos),
				plane->texturing.texture));
	return (plane->color);
}

//get the color of an object at a specific point
// if needed do the vec3 substract for the plane as well
t_color	object_color_at(t_object object, t_vec3 pt)
{
	if (object.type == SPHERE)
		return (sphere_color_at(object.sphere, pt));
	if (object.type == PLANE)
		return (plane_color_at(object.plane, pt));
	if (object.type == CYLINDER)
		return (object.cylinder->color);
	if (object.type == CONE)
		return (object.cone->color);
	return (color_new(0, 0, 0));
}

t_light	**object_lights(t_object object)
{
	if (object.type == SPHERE)
		return (object.sphere->lights);
	if (object.type == CYLINDER)
		return (object.cylinder->lights);
	if (object.type == CONE)
		return (object.cone->lights);
	return (NULL);
}
