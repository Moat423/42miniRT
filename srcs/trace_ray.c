/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:55:47 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/21 14:50:07 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color	surface_normal_color(t_ray ray, t_intersection intersection)
{
	t_color	color;

	(void)ray;
	color = object_color(intersection.object);
	(void)color;
	return vec3_multiply(vec3_add(object_normal(intersection.object, intersection.point), color_new(1,1,1)), 0.5f);
}

t_color	trace_ray(t_scene *scene, t_ray ray)
{
	t_intersection	intersection;

	(void)intersection;
	(void)scene;
	(void)ray;
	if (find_closest_intersection(scene, ray, &intersection))
		return (color_clamp(shade(scene, ray, intersection)));
		//return (calculate_lighting(intersection, scene));
	//else
	return (color_new(0, 0, 0));
}
