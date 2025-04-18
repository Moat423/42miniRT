/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:55:31 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/16 14:49:52 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

// Helper function to clamp a float value between min and max
float	clamp(float value, float min, float max)
{
	return (fminf(fmaxf(value, min), max));
}

void	objects_destroy(t_objects* objects, bool free_inner)
{
	size_t	i;

	if (!objects)
		return ;
	ssize_t ii = -1;
	if (free_inner)
	{
		while (++ii < (ssize_t)objects->sphere_count)
			free(objects->spheres[ii].lights);
	}
	free(objects->spheres);
	objects->sphere_count = 0;
	free(objects->planes);
	objects->plane_count = 0;
	i = 0;
	if (free_inner)
	{
		while (i < objects->cylinder_count)
			free(objects->cylinders[i++].lights);
	}
	free(objects->cylinders);
	objects->cylinder_count = 0;
	i = 0;
	if (free_inner)
	{
		while (i < objects->cone_count)
			free(objects->cones[i++].lights);
	}
	free(objects->cones);
	objects->cone_count = 0;
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
	scene->light_count = 0;
	objects_destroy(&scene->objs, true);
}

float	image_aspect_ratio(t_scene *scene)
{
	return ((float)scene->image_width / (float)scene->image_height);
}

bool	equal(float a, float b)
{
	return (fabs(a - b) < EPSILON);
}

// binary exponentiation
float	powi(float x, int n)
{
	float	result;

	result = 1;
	if (n < 0)
	{
		x = 1.0f / x;
		n = -n;
	}
	while (n > 0)
	{
		if (n & 1)
		{
			result *= x;
		}
		x *= x;
		n >>= 1;
	}
	return (result);
}
