/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:58:18 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/16 14:48:07 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static void	find_closest_obj_intersect(const t_objects *objs, const t_ray ray,
		t_intersection *closest, const t_object_type type)
{
	t_intersection				intersection;
	size_t						i;
	static const t_intersect_fn	intersect_fn[OBJ_NUM] = {
	[SPHERE] = (t_intersect_fn) sphere_intersect,
	[PLANE] = (t_intersect_fn) plane_intersect,
	[CYLINDER] = (t_intersect_fn) cylinder_intersect,
	[CONE] = (t_intersect_fn) cone_intersect
	};
	static const size_t			elem_size[OBJ_NUM] = {
	[SPHERE] = sizeof(t_sphere), [PLANE] = sizeof(t_plane),
	[CYLINDER] = sizeof(t_cylinder), [CONE] = sizeof(t_cone)
	};

	i = 0;
	while (i < objs->objs_n[type])
	{
		if (intersect_fn[type](objs->objs[type] + i * elem_size[type],
			ray, &intersection) && intersection.distance < closest->distance)
			*closest = intersection;
		i++;
	}
}

bool	find_closest_intersection(t_scene *scene, t_ray ray,
		t_intersection *closest)
{
	*closest = (t_intersection){0};
	closest->distance = INFINITY;
	find_closest_obj_intersect(&scene->objs, ray, closest, SPHERE);
	find_closest_obj_intersect(&scene->objs, ray, closest, PLANE);
	find_closest_obj_intersect(&scene->objs, ray, closest, CYLINDER);
	find_closest_obj_intersect(&scene->objs, ray, closest, CONE);
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
