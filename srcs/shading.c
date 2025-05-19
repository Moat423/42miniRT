/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:40:23 by kwurster          #+#    #+#             */
/*   Updated: 2025/05/15 15:45:46 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_light	**object_lights(t_object object);

static float	lambertian(t_vec3 normal, t_vec3 light_dir)
{
	return (fmaxf(0.0f, vec3_dot(normal, light_dir)));
}

static t_color	ambient(t_scene *scene, t_object object, t_vec3 pt)
{
	return (vec3_multiply(
			vec3_component_mul(scene->ambient.color, object_color_at(object, pt)),
			scene->ambient.brightness));
}

static bool	is_in_shadow(t_scene *scene, t_ray ray, t_light light)
{
	t_intersection	ix;
	size_t			i;

	i = 0;
	while (i < light.objs.sphere_count)
		if (sphere_intersect(&light.objs.spheres[i++], ray, &ix))
			return (true);
	i = 0;
	while (i < scene->objs.plane_count)
		if (plane_intersect(&scene->objs.planes[i++], ray, &ix))
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

t_color	shade(t_scene *scene, t_ray ray, t_intersection intersection)
{
	t_color		color;
	t_vec3		normal;
	size_t		i;
	t_light_ray	l;
	t_light		*light;

	color = ambient(scene, intersection.object, intersection.point);
	normal = intersect_normal(&intersection);
	i = 0;
	while (intersection.object.type != PLANE && object_lights(intersection.object)[i])
	{
		light = object_lights(intersection.object)[i];
		intersection.point = vec3_add(intersection.point, vec3_multiply(normal, EPSILON));
		l.direction = vec3_calc_length_and_normalize(
				vec3_subtract(light->pos, intersection.point),
				&(l.distance));
		l.lambert = lambertian(normal, l.direction);
		if (l.lambert > 0)
		{
			if (!is_in_shadow(scene, (t_ray){intersection.point,
					l.direction, interval_new(0, l.distance)}, *light))
				color = vec3_add(color,
						calc_lights(*light, ray, intersection, l));
		}
		i++;
	}
	while (intersection.object.type == PLANE && i < scene->light_count)
	{
		light = &scene->lights[i];
		intersection.point = vec3_add(intersection.point, vec3_multiply(normal, EPSILON));
		l.direction = vec3_calc_length_and_normalize(
				vec3_subtract(light->pos, intersection.point),
				&(l.distance));
		l.lambert = lambertian(normal, l.direction);
		if (l.lambert > 0)
		{
			if (!is_in_shadow(scene, (t_ray){intersection.point,
					l.direction, interval_new(0, l.distance)}, *light))
				color = vec3_add(color,
						calc_lights(*light, ray, intersection, l));
		}
		i++;
	}
	return (color);
}
