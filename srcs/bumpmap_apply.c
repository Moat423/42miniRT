/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bumpmap_apply.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:10:21 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/12 12:26:14 by lmeubrin         ###   ########.fr       */
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
	height = get_bump_elevation(sphere->bumpmap, point);
	// Get height differences (derivatives)
	// Sample neighboring points for derivatives
	// Using small offset for UV coordinates
	t_point point_u = point;
	t_point point_v = point;
	// Offset points slightly in u and v directions
	point_u.u = fmin(point.u + 1.0f / sphere->bumpmap->width, 1.0f);
	point_v.v = fmin(point.v + 1.0f / sphere->bumpmap->height, 1.0f);
	h_point.u = get_bump_elevation(sphere->bumpmap, point_u) - height;
	h_point.v = get_bump_elevation(sphere->bumpmap, point_v) - height;
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
	bitangent = vec3_normalize(vec3_cross(normal, tangent));
	// Bitangent (along lines of latitude)
	// float theta = M_PI * point.v;
	// bitangent = vec3_normalize((t_vec3){
	// 	cos(phi) * cos(theta),
	// 	-sin(theta),
	// 	sin(phi) * cos(theta)
	// });
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
