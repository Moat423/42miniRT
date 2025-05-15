/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bumpmap_apply.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:10:21 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/15 14:02:00 by lmeubrin         ###   ########.fr       */
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

// Generic bilinear interpolation function
// point is being reused to store the top interpolation in u and bottom
// interpolation in v
float	bilinear_interpolate(const float *data, const int width,
							const int height, const t_point uv)
{
	t_point	point;
	t_point	interpol_weight;
	int		texel_index[4];
	float	texel_data[4];

	point.u = uv.u * (width - 1);
	point.v = uv.v * (height - 1);
	texel_index[0] = (int)floorf(point.u);
	texel_index[1] = (int)floorf(point.v);
	texel_index[2] = fmin(texel_index[0] + 1, width - 1);
	texel_index[3] = fmin(texel_index[1] + 1, height - 1);
	interpol_weight.u = point.u - texel_index[0];
	interpol_weight.v = point.v - texel_index[1];
	texel_data[0] = data[texel_index[1] * width + texel_index[0]];
	texel_data[1] = data[texel_index[1] * width + texel_index[2]];
	texel_data[2] = data[texel_index[3] * width + texel_index[0]];
	texel_data[3] = data[texel_index[3] * width + texel_index[2]];
	point.u = texel_data[0] * (1 - interpol_weight.u)
		+ texel_data[1] * interpol_weight.u;
	point.v = texel_data[2] * (1 - interpol_weight.u)
		+ texel_data[3] * interpol_weight.u;
	return (point.u * (1 - interpol_weight.v) + point.v * interpol_weight.v);
}

float	get_filtered_bump_elevation(const t_bumpmap *bump, const t_point uv)
{
	return (bilinear_interpolate(bump->elevation, bump->width,
			bump->height, uv));
}

/*
** Calculate bump map normal perturbation
** Returns the perturbed normal vec3
** h_u = change in height when moving in the u direction
** h_v = change in height when moving in the v direction
*/
// t_vec3	apply_bump_mapping(const t_sphere *sphere,
// 							const t_vec3 intersection_point, t_vec3 normal)
// {
// 	t_point		point;
// 	t_vec3		tangent;
// 	t_vec3		bitangent;
// 	float		height;
// 	t_point		h_point;
//
// 	if (sphere->texture != BUMP)
// 		return (normal);
// 	point = spherical_map(intersection_point, sphere->radius);
// 	// Sample height at current point and neighboring points
// 	height = get_filtered_bump_elevation(sphere->bumpmap, point);
// 	// Get height differences (derivatives)
// 	// Using small offset for UV coordinates
// 	// float uv_step = 0.0005; // Constant step in UV space
// 	// point.u = fmod(point.u + uv_step, 1.0f); // Use fmod to handle wrapping
// 	// point.v = fmod(point.v + uv_step, 1.0f);
// 	point.u = fmin(point.u + 1.0f/sphere->bumpmap->width, 1.0f);
// 	point.v = fmin(point.v + 1.0f/sphere->bumpmap->height, 1.0f);
// 	// Offset points slightly in u and v directions
// 	h_point.u = get_filtered_bump_elevation(sphere->bumpmap, point) - height;
// 	h_point.v = get_filtered_bump_elevation(sphere->bumpmap, point) - height;
// 	// h_point.u = get_bump_elevation(sphere->bumpmap, (t_point){
// 	// 		fmin(point.u + 1.0f/sphere->bumpmap->width, 1.0f), point.v}) - height;
// 	// h_point.v = get_bump_elevation(sphere->bumpmap, (t_point){point.u, 
// 	// 		fmin(point.v + 1.0f/sphere->bumpmap->height, 1.0f)} - height);
// 	h_point.u *= BUMP_STRENGTH;
// 	h_point.v *= BUMP_STRENGTH;
// 	t_vec3 world_up = {0, 1, 0};
// 	// If normal is parallel to world_up, use a different reference vector
// 	if (fabs(vec3_dot(normal, world_up)) > 0.99f)
// 		world_up = (t_vec3){1, 0, 0};
// 	float phi = 2 * M_PI * point.u;
// 	// Primary tangent (along lines of longitude)
// 	tangent = vec3_normalize((t_vec3){
// 		-sin(phi),
// 		0,
// 		cos(phi)
// 	});
// 	// bitangent simple calc:
// 	// bitangent = vec3_normalize(vec3_cross(normal, tangent));
// 	// Bitangent (along lines of latitude)
// 	float theta = M_PI * point.v;
// 	bitangent = vec3_normalize((t_vec3){
// 		cos(phi) * cos(theta),
// 		-sin(theta),
// 		sin(phi) * cos(theta)
// 	});
// 	// Perturb the normal using the height derivatives
// 	normal = vec3_normalize(
// 			vec3_subtract(normal, 
// 				vec3_add(
// 					vec3_multiply(tangent, h_point.u),
// 					vec3_multiply(bitangent, h_point.v)
// 					)
// 				)
// 			);
// 	return (normal);
// }

// bumped surface:
// q(u,v)=point(u,v)+height(u,v)*normal(u,v)
// point is the 3d point translated to 2d coordinates u and v
// height is the height function at u and v (the computed value by which the height changed)
// normal is the normal the those 2D coordinates
t_vec3	apply_bump_mapping(const t_sphere *sphere,
							const t_vec3 intersection_point, t_vec3 normal)
{
	t_point	point;
	t_vec3	tangent;
	t_vec3	bitangent;
	float	height;
	t_point	h_point;
	float	phi;
	float	theta;
	t_vec3	world_up = {0, 1, 0};

	world_up = (t_vec3){0, 1, 0};
	if (fabs(vec3_dot(normal, world_up)) > 0.99f)
		world_up = (t_vec3){1, 0, 0};
	if (sphere->texture != BUMP)
		return (normal);
	point = spherical_map(intersection_point, sphere->radius);
	height = get_filtered_bump_elevation(sphere->bumpmap, point);
	h_point.u = get_filtered_bump_elevation(sphere->bumpmap, (t_point){
			fmin(point.u + 1.0f / sphere->bumpmap->width, 1.0f), point.v})
		- height;
	h_point.v = get_filtered_bump_elevation(sphere->bumpmap, (t_point){point.u, 
			fmin(point.v + 1.0f / sphere->bumpmap->height, 1.0f)}) - height;
	h_point.u *= BUMP_STRENGTH;
	h_point.v *= BUMP_STRENGTH;
	if (fabs(vec3_dot(normal, world_up)) > 0.99f)
		world_up = (t_vec3){1, 0, 0};
	phi = 2 * M_PI * point.u;
	tangent = vec3_normalize((t_vec3){
			-sin(phi),
			0,
			cos(phi)
		});
	theta = M_PI * point.v;
	bitangent = vec3_normalize((t_vec3){
			cos(phi) * cos(theta),
			-sin(theta),
			sin(phi) * cos(theta)
		});
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
