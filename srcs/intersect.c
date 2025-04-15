/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:58:18 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/01 14:38:05 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"



static void	find_closest_obj_intersect(t_objects *objs, t_ray ray, t_intersection *closest, t_object_type type)
{
	t_intersection	intersection;
	t_intersect_fn	intersect_fn;
	size_t			object_count;
	void			*objects;
	size_t			elem_size;
	size_t			i;

	i = 0;
	if (type == SPHERE)
	{
		intersect_fn = (t_intersect_fn)sphere_intersect;
		objects = (void *)objs->spheres;
		object_count = objs->sphere_count;
		elem_size = sizeof(t_sphere);
	}
	else if (type == PLANE)
	{
		intersect_fn = (t_intersect_fn)plane_intersect;
		objects = (void *)objs->planes;
		object_count = objs->plane_count;
		elem_size = sizeof(t_plane);
	}
	else if (type == CYLINDER)
	{
		intersect_fn = (t_intersect_fn)cylinder_intersect;
		objects = (void *)objs->cylinders;
		object_count = objs->cylinder_count;
		elem_size = sizeof(t_cylinder);
	}
	else
	{
		intersect_fn = (t_intersect_fn)cone_intersect;
		objects = (void *)objs->cones;
		object_count = objs->cone_count;
		elem_size = sizeof(t_cone);
	}
	while (i < object_count)
	{
		if (intersect_fn((void *)((char *)objects + i * elem_size), ray, &intersection)
		&& intersection.distance < closest->distance)
			*closest = intersection;
		i++;
	}
}

bool	find_closest_intersection(t_scene *scene, t_ray ray, t_intersection *closest)
{
	*closest = (t_intersection){0};
	closest->distance = INFINITY;
	find_closest_obj_intersect(&scene->objects, ray, closest, SPHERE);
	find_closest_obj_intersect(&scene->objects, ray, closest, PLANE);
	find_closest_obj_intersect(&scene->objects, ray, closest, CYLINDER);
	find_closest_obj_intersect(&scene->objects, ray, closest, CONE);
	return (closest->object.any != NULL);
}

/// @brief Lazy calculation of the surface normal of an intersection
/// @param intersection The intersection to calculate the surface normal for
/// @return The surface normal of the intersection point
t_vec3	intersect_normal(t_intersection *intersection)
{
	if (intersection->normal_calculated)
		return (intersection->normal);
	intersection->normal = vec3_normalize(intersection->normal);
	intersection->normal_calculated = true;
	return (intersection->normal);
}

static bool any_obj_intersect(t_objects *objs, t_ray ray, t_object_type type)
{
    t_intersection  intersection;
    t_intersect_fn  intersect_fn;
    size_t          object_count;
    void            *objects;
    size_t          elem_size;
    size_t          i;
    t_object        object;
    t_aabb          aabb;
    t_vec3          closest_point;
    float           sq_dist_to_aabb;
    float           sq_max_dist = ray.range.max * ray.range.max;
    static size_t	dist_culled = 0;
    static size_t	total_objs = 0;

    i = 0;
    if (type == SPHERE)
    {
        intersect_fn = (t_intersect_fn)sphere_intersect;
        objects = (void *)objs->spheres;
        object_count = objs->sphere_count;
        elem_size = sizeof(t_sphere);
    }
    else if (type == PLANE)
    {
        intersect_fn = (t_intersect_fn)plane_intersect;
        objects = (void *)objs->planes;
        object_count = objs->plane_count;
        elem_size = sizeof(t_plane);
    }
    else if (type == CYLINDER)
    {
        intersect_fn = (t_intersect_fn)cylinder_intersect;
        objects = (void *)objs->cylinders;
        object_count = objs->cylinder_count;
        elem_size = sizeof(t_cylinder);
    }
    else
    {
        intersect_fn = (t_intersect_fn)cone_intersect;
        objects = (void *)objs->cones;
        object_count = objs->cone_count;
        elem_size = sizeof(t_cone);
    }

    while (i < object_count)
    {
    	total_objs++;
        // Create an object reference to get the AABB
        object.type = type;
        object.any = (void *)((char *)objects + i * elem_size);

        // Skip AABB check for planes since they're infinite
        if (type != PLANE)
        {
            // Get AABB for this object
            aabb = create_aabb_from_object(object);

            // Find the closest point on the AABB to the ray origin
            closest_point = aabb_closest_point(aabb, ray.origin);

            // Compute squared distance from ray origin to closest point on AABB
            sq_dist_to_aabb = vec3_squared_length(vec3_subtract(closest_point, ray.origin));

            // If the AABB is further away than our maximum distance, skip this object
            if (sq_dist_to_aabb > sq_max_dist)
            {
                i++;
                dist_culled++;
                if (dist_culled % 1000000 == 0)
                {
                	printf("Culled objects because too far away from ray origin: %lu\nThat is %lu percent of total (%lu)\n", dist_culled, (dist_culled * 100) / total_objs, total_objs);
                }
                continue;
            }
        }

        // Perform the actual intersection test
        if (intersect_fn(object.any, ray, &intersection)
            && intersection.distance > 0.001f && intersection.distance < ray.range.max)
            return (true);

        i++;
    }

    return (false);
}

bool any_intersect(t_scene *scene, t_ray ray)
{
    // Check each object type for any intersection
    // Return immediately if found
    if (any_obj_intersect(&scene->objects, ray, SPHERE))
        return (true);
    if (any_obj_intersect(&scene->objects, ray, PLANE))
        return (true);
    if (any_obj_intersect(&scene->objects, ray, CYLINDER))
        return (true);
    if (any_obj_intersect(&scene->objects, ray, CONE))
        return (true);

    return (false);
}
