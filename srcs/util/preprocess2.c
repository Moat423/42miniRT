/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:39:12 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/16 14:49:52 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

/// When calling this function, we already know on a per object basis
/// which light affects which object
/// We loop through every object's lights and for each of those lights
/// add the object reference so that we can also loop through
/// the objects a light affects directly on the light struct via its objects var
bool	fill_objects_for_lights(t_scene *scene)
{
	t_objects	*objs;
	ssize_t		i;
	ssize_t		j;

	i = -1;
	while (++i < (ssize_t)scene->objs.sphere_count)
	{
		j = -1;
		while (scene->objs.spheres[i].lights[++j])
		{
			objs = &scene->objs.spheres[i].lights[j]->objs;
			objs->spheres[objs->sphere_count++] = scene->objs.spheres[i];
		}
	}
	i = -1;
	while (++i < (ssize_t)scene->objs.cylinder_count)
	{
		j = -1;
		while (scene->objs.cylinders[i].lights[++j])
		{
			objs = &scene->objs.cylinders[i].lights[j]->objs;
			objs->cylinders[objs->cylinder_count++] = scene->objs.cylinders[i];
		}
	}
	i = -1;
	while (++i < (ssize_t)scene->objs.cone_count)
	{
		j = -1;
		while (scene->objs.cones[i].lights[++j])
		{
			objs = &scene->objs.cones[i].lights[j]->objs;
			objs->cones[objs->cone_count++] = scene->objs.cones[i];
		}
	}
	return (true);
}
