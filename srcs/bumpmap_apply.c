/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bumpmap_apply.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:10:21 by lmeubrin          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/05/21 14:37:59 by lmeubrin         ###   ########.fr       */
=======
/*   Updated: 2025/05/21 14:36:38 by lmeubrin         ###   ########.fr       */
>>>>>>> cfce6d851208bc9ad173c35d5e491730b28c744e
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

static float	get_filtered_bump_elevation(const t_bumpmap *bump,
												const t_point uv)
{
	return (bilinear_interpolate(bump->elevation, bump->width,
			bump->height, uv));
}

// gets the derivative at a uv_point from a corpus
static t_point	get_derivative_bump_height(t_bumpmap *bumpmap, t_point uv_point)
{
	float	height;
	t_point	derived_p;

	height = get_filtered_bump_elevation(bumpmap, uv_point);
	derived_p.u = get_filtered_bump_elevation(bumpmap, (t_point){
			fmod(uv_point.u + 1.0f / bumpmap->width, 1.0f), uv_point.v})
		- height;
	derived_p.v = get_filtered_bump_elevation(bumpmap, (t_point){uv_point.u, 
			fmod(uv_point.v + 1.0f / bumpmap->height, 1.0f)}) - height;
	derived_p.u *= BUMP_STRENGTH;
	derived_p.v *= BUMP_STRENGTH;
	return (derived_p);
}

static t_vec3	normal_from_tangent_space(const t_point derived_p,
							const t_point uv_point, const t_vec3 normal)
{
	t_vec3	tangent;
	t_vec3	bitangent;
	float	phi;
	float	theta;
	t_vec3	tan_normal;

	phi = 2 * M_PI * uv_point.u;
	tangent = vec3_normalize((t_vec3){-sin(phi), 0, cos(phi)});
	theta = M_PI * uv_point.v;
	bitangent = vec3_normalize((t_vec3){cos(phi) * cos(theta),
			-sin(theta), sin(phi) * cos(theta)});
	tan_normal = vec3_normalize(
			vec3_subtract(normal,
				vec3_add(
					vec3_multiply(tangent, derived_p.u),
					vec3_multiply(bitangent, derived_p.v)
					)
				)
			);
	return (tan_normal);
}

/* applied a bump map on a sphere at the pixel specified in intersection_point
** Calculate bump map normal perturbation
** Returns the perturbed normal vec3
*/
t_vec3	apply_bump_mapping(const t_sphere *sphere,
							const t_vec3 intersection_point, t_vec3 normal)
{
	t_point	point;
	t_point	derived_p;

	if (!sphere->texturing.bumpmap)
		return (normal);
	point = spherical_map(intersection_point, sphere->radius);
	derived_p = get_derivative_bump_height(sphere->texturing.bumpmap, point);
	normal = normal_from_tangent_space(derived_p, point, normal);
	return (normal);
}

/* applied a bump map on a sphere at the pixel specified in intersection_point
** Calculate bump map normal perturbation
** Returns the perturbed normal vec3
*/
// t_vec3	plane_apply_bump_mapping(const t_plane *plane,
// 							const t_vec3 intersection_point, t_vec3 normal)
// {
// 	t_point	point;
// 	t_point	derived_p;
//
// 	if (!plane->texturing.bumpmap)
// 		return (normal);
// 	point = planar_map(plane->normal,
// 			vec3_subtract(intersection_point, plane->pos));
// 	point.u = (fmod(fmod(point.u, 1.0f) + 1.0, 1.0));
// 	point.v = (fmod(fmod(point.v, 1.0f) + 1.0, 1.0));
// 	derived_p = get_derivative_bump_height(plane->texturing.bumpmap, point);
// 	normal = normal_from_tangent_space(derived_p, point, normal);
// 	return (normal);
// }
