/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*													+:+ +:+		 +:+	 */
/*   By: lmeubrin <lmeubrin@student.42berlin.d	  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/04/29 16:28:11 by lmeubrin		  #+#	#+#			 */
/*   Updated: 2025/04/30 11:50:31 by lmeubrin         ###   ########.fr       */
/*																			*/
/* ************************************************************************** */

#include "../include/miniRT_types.h"
#include <math.h>

t_checkers	uv_checkers(int width, int height, t_color color_a, t_color color_b)
{
	t_checkers	ch;

	ch.width = width;
	ch.height = height;
	ch.color_a = color_a;
	ch.color_b = color_b;
	return (ch);
}

t_checkers	default_uv_checkers(void)
{
	t_checkers	ch;

	ch.width = 16;
	ch.height = 8;
	ch.color_a = (t_vec3){255, 255, 255};
	ch.color_b = (t_vec3){0, 0, 0};
	return (ch);
}

// returns the color at a point defined by
// its x coordinate u and y coordinate v, to not confuse, as these are 2D coords
// this function is optimized for speed and will fail for negative floats,
// but its fast.
t_color	uv_pattern_at(const t_checkers checkers, t_point pt)
{
	pt.min *= checkers.width;
	pt.max *= checkers.height;
	if (((int)pt.min + (int)pt.max) & 1)
		return (checkers.color_a);
	return (checkers.color_b);
}

t_point	spherical_map(const t_vec3 sphere_point, const float radius)
{
	float	theta;
	float	phi;
	float	raw_u;
	t_point	pt;

	theta = atan2(sphere_point.x, sphere_point.z);
	phi = acos(sphere_point.y / radius);
	raw_u = theta / (2 * M_PI);
	pt.min = 1 - (raw_u + 0.5);
	pt.max = 1 - phi / M_PI;
	return (pt);
}

t_color	pattern_at(const t_vec3 sphere_point, const float radius)
{
	t_checkers	checkers;
	t_point		pt;

	checkers = default_uv_checkers();
	pt = spherical_map(sphere_point, radius);
	return (uv_pattern_at(checkers, pt));
}
