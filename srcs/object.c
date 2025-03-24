/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:51:50 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/24 19:25:15 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color	object_color(t_object object)
{
	if (object.type == SPHERE)
		return (object.sphere->color);
	if (object.type == PLANE)
		return (object.plane->color);
	if (object.type == CYLINDER)
		return (object.cylinder->color);
	return (color_new(0, 0, 0));
}

t_vec3	object_normal(t_object object, t_vec3 point)
{
	if (object.type == SPHERE)
		return (sphere_normal(*object.sphere, point));
	else if (object.type == PLANE)
		return (object.plane->normal);
	// else if (object.type == CYLINDER)
	// 	return (object.cylinder->axis);
	return (vec3_new(0, 0, 0));
}
