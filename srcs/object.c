/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:51:50 by kwurster          #+#    #+#             */
/*   Updated: 2025/05/15 15:49:57 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color	object_color_at(t_object object, t_vec3 pt)
{
	if (object.type == SPHERE)
	{
		if (object.sphere->texture == CHECKERS)
			return (sphere_pattern_at(vec3_subtract(pt, object.sphere->pos),
			object.sphere->radius));
		return (object.sphere->color);
	}
	if (object.type == PLANE)
	{
		if (object.plane->texture == CHECKERS)
			return (planar_pattern_at(pt));
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
