/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_destroy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:30:26 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/28 12:30:44 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT_types.h"

static void	objects_destroy_inner(t_objects *objects)
{
	size_t	i;

	if (!objects)
		return ;
	i = 0;
	while (i < objects->sphere_count)
		free(objects->spheres[i++].lights);
	i = 0;
	while (i < objects->cylinder_count)
		free(objects->cylinders[i++].lights);
	i = 0;
	while (i < objects->cone_count)
		free(objects->cones[i++].lights);
}

void	objects_destroy(t_objects *objects, bool free_inner)
{
	if (!objects)
		return ;
	if (free_inner)
		objects_destroy_inner(objects);
	free(objects->planes);
	objects->plane_count = 0;
	free(objects->spheres);
	objects->sphere_count = 0;
	free(objects->cylinders);
	objects->cylinder_count = 0;
	free(objects->cones);
	objects->cone_count = 0;
	ft_bzero(&(objects->objs_n), sizeof(size_t) * 4);
}

/// Free memory allocated for a scene
void	scene_destroy(t_scene *scene)
{
	size_t	i;

	if (!scene)
		return ;
	i = 0;
	while (i < scene->light_count)
		objects_destroy(&scene->lights[i++].objs, false);
	free(scene->lights);
	if (scene->light_count == 0)
		objects_destroy(&scene->objs, false);
	else
	{
		scene->light_count = 0;
		objects_destroy(&scene->objs, true);
	}
}

