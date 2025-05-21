/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:40:23 by kwurster          #+#    #+#             */
/*   Updated: 2025/05/22 15:57:23 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_light	**object_lights(t_object object);

static float	lambertian(t_vec3 normal, t_vec3 light_dir)
{
	return (fmaxf(0.0f, vec3_dot(normal, light_dir)));
}

static t_color	ambient(t_scene *scene, t_object object)
{
	return (vec3_multiply(
			vec3_component_mul(scene->ambient.color, object_color(object)),
			scene->ambient.brightness));
}

static bool	is_in_shadow(t_scene *scene, t_ray ray, t_light light)
{
	t_intersection	ix;
	size_t			i;

	i = 0;
	while (i < scene->objs.plane_count)
		if (plane_intersect(&scene->objs.planes[i++], ray, &ix))
			return (true);
	i = 0;
	while (i < light.objs.sphere_count)
		if (sphere_intersect(&light.objs.spheres[i++], ray, &ix))
			return (true);
	i = 0;
	while (i < light.objs.cylinder_count)
		if (cylinder_intersect(&light.objs.cylinders[i++], ray, &ix))
			return (true);
	i = 0;
	while (i < light.objs.cone_count)
		if (cone_intersect(&light.objs.cones[i++], ray, &ix))
			return (true);
	return (false);
}

static t_color	light_contribution(t_scene *scene, t_intersection ix,
		t_ray ray, t_light light)
{
	t_light_ray	l_ray;

	l_ray.direction = vec3_calc_length_and_normalize(
			vec3_subtract(light.pos, ix.point), &(l_ray.distance));
	l_ray.lambert = lambertian(ix.normal, l_ray.direction);
	if (l_ray.lambert > 0)
	{
		if (!is_in_shadow(scene, (t_ray){ix.point,
				l_ray.direction, interval_new(0.005, l_ray.distance)}, light))
			return (calc_lights(light, ray, ix, l_ray));
	}
	return (color_new(0, 0, 0));
}

t_color	shade(t_scene *scene, t_ray ray, t_intersection ix)
{
	t_color		color;
	size_t		i;
	t_light		**obj_lights;

	color = ambient(scene, ix.object);
	i = 0;
	obj_lights = object_lights(ix.object);
	intersect_normal(&ix);
	if (obj_lights)
	{
		while (obj_lights[i])
		{
			color = vec3_add(color, light_contribution(scene, ix, ray,
						*(obj_lights[i++])));
		}
	}
	else
	{
		while (i < scene->light_count)
		{
			color = vec3_add(color, light_contribution(scene, ix, ray,
						scene->lights[i++]));
		}
	}
	return (color);
}
