/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:58:18 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/01 14:38:05 by lmeubrin         ###   ########.fr       */
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
		objects = (void *)scene->spheres;
		object_count = scene->sphere_count;
		elem_size = sizeof(t_sphere);
	}
	else if (type == PLANE)
	{
		intersect_fn = (t_intersect_fn)plane_intersect;
		objects = (void *)scene->planes;
		object_count = scene->plane_count;
		elem_size = sizeof(t_plane);
	}
	else if (type == CYLINDER)
	{
		intersect_fn = (t_intersect_fn)cylinder_intersect;
		objects = (void *)scene->cylinders;
		object_count = scene->cylinder_count;
		elem_size = sizeof(t_cylinder);
	}
	else 
	{
		intersect_fn = (t_intersect_fn)cone_intersect;
		objects = (void *)scene->cones;
		object_count = scene->cone_count;
		elem_size = sizeof(t_cone);
	}
	while (i < object_count)
	{
		if (intersect_fn((void *)((char *)objects + i * elem_size), ray, &intersection)
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
	find_closest_obj_intersect(scene, ray, closest, CYLINDER);
	find_closest_obj_intersect(scene, ray, closest, CONE);
	return (closest->object.any != NULL);
}

/// @brief Lazy calculation of the surface normal of an intersection
/// @param intersection The intersection to calculate the surface normal for
/// @return The surface normal of the intersection point
t_vec3	intersect_normal(t_intersection *intersection)
{
	if (intersection->normal_calculated)
		return (intersection->normal);
	intersection->normal = vec3_normalize(intersection->normal);
	intersection->normal_calculated = true;
	return (intersection->normal);
}
