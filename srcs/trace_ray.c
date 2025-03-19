/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:55:47 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/19 12:31:24 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color	color_of_object(t_intersection intersection)
{
	if (intersection.type == SPHERE)
		return (((t_sphere*)intersection.object)->color);
	if (intersection.type == PLANE)
		return (((t_plane*)intersection.object)->color);
	if (intersection.type == CYLINDER)
		return (((t_cylinder*)intersection.object)->color);
	return (color_new(0, 0, 0));
}

t_color	trace_ray(t_scene *scene, t_ray ray)
{
	t_intersection	intersection;

	(void)intersection;
	(void)scene;
	(void)ray;
	intersection = find_closest_intersection(scene, ray);
	if (intersection.object != NULL)
		return (color_of_object(intersection));
		//return (calculate_lighting(intersection, scene));
	//else
	return (color_new(0, 0, 0));
}
