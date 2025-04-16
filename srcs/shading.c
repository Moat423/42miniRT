/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:40:23 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/16 14:49:52 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static float	lambertian(t_vec3 normal, t_vec3 light_dir)
{
	return (fmaxf(0.0f, vec3_dot(normal, light_dir)));
}

// OBSOLETE FUNCTION RIGHT HERE!!!!!!!!!!!
// // Diffuse = Kd * Id * max(0, N·L)
// static t_color	light_diffuse(t_light light, t_object object, float lambertian,
// 							float light_dist_sq)
// {
// 	t_color	color;
//
// 	color = vec3_component_mul(object_color(object), light.color);
// 	color = vec3_multiply(color, lambertian *
// 			light.brightness / (light_dist_sq / 5.0));
// 	return (color);
// }

static t_color	ambient(t_scene *scene, t_object object)
{
	return (vec3_multiply(
			vec3_component_mul(scene->ambient.color, object_color(object)),
			scene->ambient.brightness));
}

bool	any_intersect(t_scene *scene, t_ray ray);

static bool is_in_shadow(t_scene *scene, t_ray ray, t_light light)
{
    t_intersection   intersection;
    size_t           i;
    void            *object_ptr;

    // Check only objects that are associated with this light
    // For spheres
    i = 0;
    while (i < light.objs.sphere_count)
    {
        object_ptr = (void *)&light.objs.spheres[i];
        if (sphere_intersect(object_ptr, ray, &intersection) &&
            intersection.distance > 0.001f && intersection.distance < ray.range.max)
            return (true);
        i++;
    }

    // For planes (we need to check all planes since they don't have bounding boxes)
    i = 0;
    while (i < scene->objs.plane_count)
    {
        object_ptr = (void *)&scene->objs.planes[i];
        if (plane_intersect(object_ptr, ray, &intersection) &&
            intersection.distance > 0.001f && intersection.distance < ray.range.max)
            return (true);
        i++;
    }

    // For cylinders
    i = 0;
    while (i < light.objs.cylinder_count)
    {
        object_ptr = (void *)&light.objs.cylinders[i];
        if (cylinder_intersect(object_ptr, ray, &intersection) &&
            intersection.distance > 0.001f && intersection.distance < ray.range.max)
            return (true);
        i++;
    }

    // For cones
    i = 0;
    while (i < light.objs.cone_count)
    {
        object_ptr = (void *)&light.objs.cones[i];
        if (cone_intersect(object_ptr, ray, &intersection) &&
            intersection.distance > 0.001f && intersection.distance < ray.range.max)
            return (true);
        i++;
    }

    return (false);
}

t_light	**object_lights(t_object object);

t_color	shade(t_scene *scene, t_ray ray, t_intersection intersection)
{
	t_color		color;
	t_vec3		normal;
	size_t		i;
	t_light_ray	l;
	t_light		*light;

	(void) ray;
	color = ambient(scene, intersection.object);
	normal = intersect_normal(&intersection);
	i = 0;
	while (intersection.object.type != PLANE && object_lights(intersection.object)[i])
	{
		light = object_lights(intersection.object)[i];
		l.direction = vec3_calc_length_and_normalize(
				vec3_subtract(light->pos, intersection.point),
				&(l.distance));
		l.lambert = lambertian(normal, l.direction);
		if (l.lambert > 0)
		{
			// TODO reevaluate if these ray calculations are always correct. Is the normal always in the correct direction? Do we want ray min and max to be epsilon and distance? why not 0 and distance - epsilon to accurately prevent accidental intersection with the source?
			if (!is_in_shadow(scene, (t_ray){vec3_add(intersection.point, vec3_multiply(normal, EPSILON)),
					l.direction, interval_new(EPSILON, l.distance)}, *light))
				color = vec3_add(color,
						calc_lights(*light, ray, intersection, l));
		}
		i++;
	}
	while (intersection.object.type == PLANE && i < scene->light_count)
	{
		light = &scene->lights[i];
		l.direction = vec3_calc_length_and_normalize(
				vec3_subtract(light->pos, intersection.point),
				&(l.distance));
		l.lambert = lambertian(normal, l.direction);
		if (l.lambert > 0)
		{
			// TODO reevaluate if these ray calculations are always correct. Is the normal always in the correct direction? Do we want ray min and max to be epsilon and distance? why not 0 and distance - epsilon to accurately prevent accidental intersection with the source?
			if (!is_in_shadow(scene, (t_ray){vec3_add(intersection.point, vec3_multiply(normal, EPSILON)),
					l.direction, interval_new(EPSILON, l.distance)}, *light))
				color = vec3_add(color,
						calc_lights(*light, ray, intersection, l));
		}
		i++;
	}
	return (color);
}
