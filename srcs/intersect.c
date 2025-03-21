/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:58:18 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/21 13:43:10 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

void	find_closest_sphere_intersection(t_scene *scene, t_ray ray, t_intersection *closest)
{
	t_intersection	intersection;
	size_t			i;

	i = 0;
	while (i < scene->sphere_count)
	{
		if (sphere_intersect(&scene->spheres[i], ray, &intersection)
		&& intersection.distance < closest->distance)
			*closest = intersection;
		i++;
	}
}

bool	find_closest_intersection(t_scene *scene, t_ray ray, t_intersection *closest)
{
	*closest = (t_intersection){0};
	closest->distance = INFINITY;
	find_closest_sphere_intersection(scene, ray, closest);
	return (closest->object.any != NULL);
}
