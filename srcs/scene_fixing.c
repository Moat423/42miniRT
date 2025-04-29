/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_fixing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:59:10 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/29 15:53:29 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static void	set_derived_camera(t_scene *scene)
{
	static const t_vec3	world_up = {0, 1, 0};

	scene->camera.right = vec3_cross(scene->camera.dir, world_up);
	if (vec3_squared_length(scene->camera.right) < EPSILON * EPSILON)
		scene->camera.right = vec3_cross(scene->camera.dir, vec3_new(1, 0, 0));
	scene->camera.right = vec3_normalize(scene->camera.right);
	scene->camera.up = vec3_normalize(vec3_cross(scene->camera.right,
				scene->camera.dir));
}

static void	set_derived_cylinders(t_scene *scene)
{
	size_t				i;
	t_cylinder			*cyl;

	i = 0;
	while (i < scene->objs.cylinder_count)
	{
		cyl = &scene->objs.cylinders[i];
		cyl->top = vec3_add(cyl->pos, vec3_multiply(cyl->axis,
					cyl->height / 2.0f));
		cyl->bottom = vec3_add(cyl->pos, vec3_multiply(cyl->axis,
					-cyl->height / 2.0f));
		i++;
	}
}

/**
 * @brief Recalculates scene values that depend on other parsed values,
 *        particularly those affected by vector normalization.
 * @brief This should be called *after* fix_unnormalized_scene_vectors to ensure
 *        calculations use the corrected, normalized vectors.
 * @param scene A pointer to the t_scene structure to modify.
 */
void	calculate_derived_scene_values(t_scene *scene)
{
	size_t				i;
	t_cone				*cone;

	set_derived_camera(scene);
	set_derived_cylinders(scene);
	i = 0;
	while (i < scene->objs.cone_count)
	{
		cone = &scene->objs.cones[i];
		cone->bottom = vec3_add(cone->top, vec3_multiply(cone->axis,
					cone->height));
		if (cone->height > EPSILON)
			cone->slant = cone->radius / cone->height;
		else
			cone->slant = 0.0f;
		i++;
	}
}

/**
 * @brief Ensures all direction/axis/normal vectors within the scene
 *        that are expected to be normalized are re-normalized.
 * @brief This acts as a safeguard against potentially non-normalized
 *        input from the scene file.
 * @param scene A pointer to the t_scene structure to modify.
 */
void	fix_unnormalized_scene_vectors(t_scene *scene)
{
	size_t	i;

	scene->camera.dir = vec3_normalize(scene->camera.dir);
	i = 0;
	while (i < scene->objs.plane_count)
	{
		scene->objs.planes[i].normal
			= vec3_normalize(scene->objs.planes[i].normal);
		i++;
	}
	i = 0;
	while (i < scene->objs.cylinder_count)
	{
		scene->objs.cylinders[i].axis
			= vec3_normalize(scene->objs.cylinders[i].axis);
		i++;
	}
	i = 0;
	while (i < scene->objs.cone_count)
	{
		scene->objs.cones[i].axis
			= vec3_normalize(scene->objs.cones[i].axis);
		i++;
	}
}
