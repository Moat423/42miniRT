/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bumpmap_apply.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:10:21 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/12 17:11:23 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// Gets the elevation value from the bump map at given UV coordinates
float	get_bump_elevation(const t_bumpmap *bump, const t_point point)
{
	size_t	x;
	size_t	y;
	size_t	index;

	x = (size_t)(point.u * (bump->width - 1));
	y = (size_t)(point.v * (bump->height - 1));
	if (x >= bump->width)
		x = bump->width - 1;
	if (y >= bump->height)
		y = bump->height - 1;
	index = y * bump->width + x;
	return (bump->elevation[index]);
}

// Bilinear sampling function
float get_filtered_bump_elevation(const t_bumpmap *bump, const t_point uv)
{
    float u = uv.u * (bump->width - 1);
    float v = uv.v * (bump->height - 1);
    
    // Get integer coordinates
    int x0 = (int)floorf(u);
    int y0 = (int)floorf(v);
    int x1 = fmin(x0 + 1, bump->width - 1);
    int y1 = fmin(y0 + 1, bump->height - 1);
    
    // Get fractional parts
    float u_frac = u - x0;
    float v_frac = v - y0;
    
    // Sample all four points
    float h00 = bump->elevation[y0 * bump->width + x0];
    float h10 = bump->elevation[y0 * bump->width + x1];
    float h01 = bump->elevation[y1 * bump->width + x0];
    float h11 = bump->elevation[y1 * bump->width + x1];
    
    // Bilinear interpolation
    float h0 = h00 * (1 - u_frac) + h10 * u_frac;
    float h1 = h01 * (1 - u_frac) + h11 * u_frac;
    
    return h0 * (1 - v_frac) + h1 * v_frac;
}


/*
** Calculate bump map normal perturbation
** Returns the perturbed normal vec3
** h_u = change in height when moving in the u direction
** h_v = change in height when moving in the v direction
*/
t_vec3	apply_bump_mapping(const t_sphere *sphere,
							const t_vec3 intersection_point, t_vec3 normal)
{
	t_point		point;
	t_vec3		tangent;
	t_vec3		bitangent;
	float		height;
	t_point		h_point;

	if (sphere->texture != BUMP)
		return (normal);
	point = spherical_map(intersection_point, sphere->radius);
	// Sample height at current point and neighboring points
	height = get_filtered_bump_elevation(sphere->bumpmap, point);
	// Get height differences (derivatives)
	// Using small offset for UV coordinates
	t_point point_u = point;
	t_point point_v = point;
	// float uv_step = 0.0005; // Constant step in UV space
	// point_u.u = fmod(point.u + uv_step, 1.0f); // Use fmod to handle wrapping
	// point_v.v = fmod(point.v + uv_step, 1.0f);
	point_u.u = fmin(point.u + 1.0f/sphere->bumpmap->width, 1.0f);
	point_v.v = fmin(point.v + 1.0f/sphere->bumpmap->height, 1.0f);
	// Offset points slightly in u and v directions
	h_point.u = get_filtered_bump_elevation(sphere->bumpmap, point_u) - height;
	h_point.v = get_filtered_bump_elevation(sphere->bumpmap, point_v) - height;
	// h_point.u = get_bump_elevation(sphere->bumpmap, (t_point){
	// 		fmin(point.u + 1.0f/sphere->bumpmap->width, 1.0f), point.v}) - height;
	// h_point.v = get_bump_elevation(sphere->bumpmap, (t_point){point.u, 
	// 		fmin(point.v + 1.0f/sphere->bumpmap->height, 1.0f)} - height);
	h_point.u *= BUMP_STRENGTH;
	h_point.v *= BUMP_STRENGTH;
	t_vec3 world_up = {0, 1, 0};
	// If normal is parallel to world_up, use a different reference vector
	if (fabs(vec3_dot(normal, world_up)) > 0.99f)
		world_up = (t_vec3){1, 0, 0};
	float phi = 2 * M_PI * point.u;
	// Primary tangent (along lines of longitude)
	tangent = vec3_normalize((t_vec3){
		-sin(phi),
		0,
		cos(phi)
	});
	// bitangent simple calc:
	// bitangent = vec3_normalize(vec3_cross(normal, tangent));
	// Bitangent (along lines of latitude)
	float theta = M_PI * point.v;
	bitangent = vec3_normalize((t_vec3){
		cos(phi) * cos(theta),
		-sin(theta),
		sin(phi) * cos(theta)
	});
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

//bumped surface:
//q(u,v)=point(u,v)+height(u,v)*normal(u,v)
//point is the 3d point translated to 2d coordinates u and v
//height is the height function at u and v (the computed value by which the height changed)
//normal is the normal the those 2D coordinates
//
// t_vec3	apply_bump_mapping(const t_sphere *sphere,
// 							const t_vec3 intersection_point, t_vec3 normal)
// {
// 	t_point		point;
// 	float		height;
// 	t_point		h_point;
// 	float		u;
// 	float		v;
//
// 	if (sphere->texture != BUMP)
// 		return (normal);
//
// 	//new normal : cross product of partial derivatives (d for derivate)
// 	//the derivates describe the changes of the bumped surface and we cross those changes in the 2 directions to get out new normal
// 	//new_normal = (dq/dv) cross (dq/du)
// 	return (normal);
// }
