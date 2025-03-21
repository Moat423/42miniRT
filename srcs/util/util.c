/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:55:31 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/21 11:10:33 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

// Helper function to clamp a float value between min and max
float	clamp(float value, float min, float max)
{
	return (fminf(fmaxf(value, min), max));
}

/// Free memory allocated for a scene
void	scene_destroy(t_scene *scene)
{
	if (!scene)
		return ;
	free(scene->lights);
	scene->light_count = 0;
	free(scene->spheres);
	scene->sphere_count = 0;
	free(scene->planes);
	scene->plane_count = 0;
	free(scene->cylinders);
	scene->cylinder_count = 0;
}

float	image_aspect_ratio(t_scene *scene)
{
	return ((float)scene->image_width / (float)scene->image_height);
}

bool	equal(float a, float b)
{
	return (fabs(a - b) < EPSILON);
}
