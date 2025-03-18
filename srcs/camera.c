/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:55:42 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/18 14:21:36 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

float	get_height_factor(t_camera *cam)
{
	float	half_fov;

	/// (degrees to radians) / 2
	half_fov = (cam->fov * (M_PI / 180.0)) / 2;
	/// height factor
	return (tanf(half_fov));
}

/// @brief Generate a ray through a pixel on the viewport
/// @param cam The camera
/// @param u 0-1 coordinate on the x-axis, 0 being the left of the viewport
/// @param v 0-1 coordinate on the y-axis, 0 being the top of the viewport
/// @return The ray
t_ray	get_viewport_ray(t_scene *scene, float u, float v)
{
	t_ray		ray;
	t_vec3		view_u;
	t_vec3		view_v;
	float		view_width;
	float		view_height;

	/// Flip v coordinate to not produce a upside down mirrored image
	v = 1.0 - v;
	ray.origin = scene->camera.pos;
	ray.direction = scene->camera.dir;
	view_height = 2 * get_height_factor(&scene->camera);
	view_width = view_height * image_aspect_ratio(scene);
	// Calculate offset from center of viewport
	view_u = vec3_multiply(scene->camera.right, view_width * (u - 0.5f));
	view_v = vec3_multiply(scene->camera.up, view_height * (v - 0.5f));
	// Apply offset to our ray direction
	ray.direction = vec3_add(ray.direction, vec3_add(view_u, view_v));
	ray.direction = vec3_normalize(ray.direction);

	return (ray);
}
