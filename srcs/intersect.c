/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:58:18 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/19 12:56:58 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_intersection	find_closest_sphere_intersection(t_scene *scene, t_ray ray)
{
	t_intersection	intersection;
	t_intersection	closest;
	size_t			i;

	closest = (t_intersection){0};
	closest.distance = INFINITY;
	i = 0;
	while (i < scene->sphere_count)
	{
		intersection = sphere_intersect(&scene->spheres[i], ray);
		if (intersection.object.any && intersection.distance < closest.distance)
			closest = intersection;
		i++;
	}
	return (closest);
}

t_intersection	find_closest_intersection(t_scene *scene, t_ray ray)
{
	t_intersection	intersection;
	t_intersection	closest;

	(void)intersection;
	closest = find_closest_sphere_intersection(scene, ray);
	//intersection = find_closest_plane_intersection(scene, ray);
	//if (intersection.object && intersection.distance < closest.distance)
	//	closest = intersection;
	return (closest);
}
