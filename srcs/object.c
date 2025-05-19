/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:51:50 by kwurster          #+#    #+#             */
/*   Updated: 2025/05/19 13:46:29 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

//get the color of an object at a specific point
// if needed do the vec3 substract for the plane as well
t_color	object_color_at(t_object object, t_vec3 pt)
{
	if (object.type == SPHERE)
	{
		if (object.sphere->texturing.type == CHECKERS)
			return (sphere_pattern_at(
					vec3_subtract(pt, object.sphere->pos),
					object.sphere->radius, object.sphere->texturing.checkers));
		return (object.sphere->color);
	}
	if (object.type == PLANE)
	{
		if (object.plane->texturing.type == CHECKERS)
			return (planar_pattern_at(object.plane->normal, pt,
					object.plane->texturing.checkers));
		return (object.plane->color);
	}
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
