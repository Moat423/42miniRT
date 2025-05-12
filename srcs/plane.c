/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:51:03 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/12 15:05:56 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static t_point repeated_planar_map(const t_vec3 point, const t_plane *plane)
{
    t_point result;
    t_vec3 relative_pos;
    t_vec3 tangent, bitangent;
    float scale = 0.001f; // Adjust this to control texture size

    // Calculate position relative to plane origin
    relative_pos = vec3_subtract(point, plane->pos);

    // Create consistent tangent space
    t_vec3 world_up = {0, 1, 0};
    if (fabs(vec3_dot(plane->normal, world_up)) > 0.99f)
        world_up = (t_vec3){1, 0, 0};

    tangent = vec3_normalize(vec3_cross(world_up, plane->normal));
    bitangent = vec3_normalize(vec3_cross(plane->normal, tangent));

    // Project onto tangent space to get UV coordinates
    float u = vec3_dot(relative_pos, tangent) * scale;
    float v = vec3_dot(relative_pos, bitangent) * scale;

    // For non-repeating texture:
    // Map to 0-1 range based on texture size
    if (plane->bumpmap) {
        float width_world_units = plane->bumpmap->width / 10.0f; // Arbitrary scaling
        float height_world_units = plane->bumpmap->height / 10.0f;

        // Center the texture on the plane origin
        result.u = (u + width_world_units/2) / width_world_units;
        result.v = (v + height_world_units/2) / height_world_units;

        // Clamp to texture bounds for non-repeating texture
        result.u = fmax(0.0f, fmin(result.u, 1.0f));
        result.v = fmax(0.0f, fmin(result.v, 1.0f));
    }
    else {
        // For repeating textures, use modulo (similar to your current approach)
        result.u = u - floorf(u);
        result.v = v - floorf(v);
    }

    return result;
}

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
	point = repeated_planar_map(intersection_point, plane);
	// Sample height at current point and neighboring points
	height = get_filtered_bump_elevation(plane->bumpmap, point);
	// Get height differences (derivatives)
	// Sample neighboring points for derivatives
	// Using small offset for UV coordinates
	t_point point_u = point;
	t_point point_v = point;
	// Offset points slightly in u and v directions
	// float uv_step = 0.0001; // Constant step in UV space
	// point_u.u = fmod(point.u + uv_step, 1.0f); // Use fmod to handle wrapping
	// point_v.v = fmod(point.v + uv_step, 1.0f);
	point_u.u = fmin(point.u + 1.0f / plane->bumpmap->width, 1.0f);
	point_v.v = fmin(point.v + 1.0f / plane->bumpmap->height, 1.0f);
	h_point.u = get_filtered_bump_elevation(plane->bumpmap, point_u) - height;
	h_point.v = get_filtered_bump_elevation(plane->bumpmap, point_v) - height;
	// h_point.u = get_bump_elevation(plane->bumpmap, (t_point){
	// 		fmin(point.u + 1.0f/plane->bumpmap->width, 1.0f), point.v}) - height;
	// h_point.v = get_bump_elevation(plane->bumpmap, (t_point){point.u, 
	// 		fmin(point.v + 1.0f/plane->bumpmap->height, 1.0f) - height});
	h_point.u *= BUMP_STRENGTH;
	h_point.v *= BUMP_STRENGTH;
	// Create tangent and bitangent vec3s
	// For a sphere, we can use the spherical coordinates
    // Create tangent space
    t_vec3 world_up = {0, 1, 0};
    if (fabs(vec3_dot(normal, world_up)) > 0.99f)
        world_up = (t_vec3){1, 0, 0};
    
    tangent = vec3_normalize(vec3_cross(world_up, normal));
    bitangent = vec3_normalize(vec3_cross(normal, tangent));
	normal = vec3_normalize(
			vec3_subtract(normal, 
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
		// out->normal = plane->normal;
		out->normal = plane_apply_bump_mapping(plane, out->point, out->normal);
		out->normal_calculated = true;
		if (plane->texture == CHECKERS)
			out->color = planar_pattern_at(out->point);
		else
			out->color = plane->color;
		return (true);
	}
	return (false);
}
