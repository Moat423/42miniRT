/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:58:18 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/24 16:44:24 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static void	find_closest_obj_intersect(t_scene *scene, t_ray ray, t_intersection *closest, t_object_type type)
{
	t_intersection	intersection;
	t_intersect_fn	intersect_fn;
	size_t			object_count;
	void			*objects;
	size_t			elem_size;
	size_t			i;

	i = 0;
	if (type == SPHERE)
	{
		intersect_fn = (t_intersect_fn)sphere_intersect;
		objects = (void*)scene->spheres;
		object_count = scene->sphere_count;
		elem_size = sizeof(t_sphere);
	}
	else //if (type == PLANE)
	{
		intersect_fn = (t_intersect_fn)plane_intersect;
		objects = (void*)scene->planes;
		object_count = scene->plane_count;
		elem_size = sizeof(t_plane);
	}
	while (i < object_count)
	{
		if (intersect_fn((void*)((char*)objects + i * elem_size), ray, &intersection)
		&& intersection.distance < closest->distance)
			*closest = intersection;
		i++;
	}
}

bool	find_closest_intersection(t_scene *scene, t_ray ray, t_intersection *closest)
{
	*closest = (t_intersection){0};
	closest->distance = INFINITY;
	find_closest_obj_intersect(scene, ray, closest, SPHERE);
	find_closest_obj_intersect(scene, ray, closest, PLANE);
	return (closest->object.any != NULL);
}
