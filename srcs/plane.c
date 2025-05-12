/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:51:03 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/12 11:08:10 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"


/*
** Calculate bump map normal perturbation
** Returns the perturbed normal vec3
** h_u = change in height when moving in the u direction
** h_v = change in height when moving in the v direction
*/
t_vec3	plane_apply_bump_mapping(const t_plane *plane,
							const t_vec3 intersection_point, t_vec3 normal)
{
	t_point		point;
	t_vec3		tangent;
	t_vec3		bitangent;
	float		height;
	t_point		h_point;

	if (plane->texture != BUMP)
		return (normal);
	point = planar_map(intersection_point);
	// Sample height at current point and neighboring points
	height = get_bump_elevation(plane->bumpmap, point);
	// Get height differences (derivatives)
	// Sample neighboring points for derivatives
	// Using small offset for UV coordinates
	t_point point_u = point;
	t_point point_v = point;
	// Offset points slightly in u and v directions
	point_u.u = fmin(point.u + 1.0f / plane->bumpmap->width, 1.0f);
	point_v.v = fmin(point.v + 1.0f / plane->bumpmap->height, 1.0f);
	h_point.u = get_bump_elevation(plane->bumpmap, point_u) - height;
	h_point.v = get_bump_elevation(plane->bumpmap, point_v) - height;
	// h_point.u = get_bump_elevation(sphere->bumpmap, (t_point){
	// 		fmin(point.u + 1.0f/sphere->bumpmap->width, 1.0f), point.v}) - height;
	// h_point.v = get_bump_elevation(sphere->bumpmap, (t_point){point.u, 
	// 		fmin(point.v + 1.0f/sphere->bumpmap->height, 1.0f)} - height);
	h_point.u *= BUMP_STRENGTH;
	h_point.v *= BUMP_STRENGTH;
	// Create tangent and bitangent vec3s
	// For a sphere, we can use the spherical coordinates
	tangent = vec3_normalize((t_vec3){-sin(2 * M_PI * point.u), 0, 
										cos(2 * M_PI * point.u)});
	bitangent = vec3_normalize(vec3_cross(normal, tangent));
	// Perturb the normal using the height derivatives
	normal = vec3_normalize(
			vec3_add(normal, 
				vec3_add(
					vec3_multiply(tangent, h_point.u),
					vec3_multiply(bitangent, h_point.v)
					)
				)
			);
	return (normal);
}

bool	plane_intersect(t_plane *plane, t_ray ray, t_intersection *out)
{
	float	t;
	float	denom;

	denom = vec3_dot(plane->normal, ray.direction);
	if (denom > EPSILON || denom < -EPSILON)
	{
		t = vec3_dot(vec3_subtract(plane->pos, ray.origin),
				plane->normal) / denom;
		if (!interval_contains(ray.range, t))
			return (false);
		out->distance = t;
		out->object.plane = plane;
		out->object.type = PLANE;
		out->point = vec3_add(ray.origin, vec3_multiply(ray.direction, t));
		// out->normal = plane_apply_bump_mapping(plane, out->point, out->normal);
		out->normal = plane->normal;
		out->normal_calculated = true;
		if (plane->texture == CHECKERS)
			out->color = planar_pattern_at(out->point);
		else
			out->color = plane->color;
		return (true);
	}
	return (false);
}
