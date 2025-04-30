/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*													+:+ +:+		 +:+	 */
/*   By: lmeubrin <lmeubrin@student.42berlin.d	  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/04/29 16:28:11 by lmeubrin		  #+#	#+#			 */
/*   Updated: 2025/04/30 14:40:34 by lmeubrin         ###   ########.fr       */
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

t_checkers	plane_uv_checkers(void)
{
	t_checkers	ch;

	ch.width = 2;
	ch.height = 2;
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
	int	u2;
	int	v2;

	u2 = (int)floor(pt.min * checkers.width);
	v2 = (int)floor(pt.max * checkers.height);
	if ((u2 + v2) & 1)
		return (checkers.color_a);
	return (checkers.color_b);
}

t_point	planar_map(const t_vec3 plane_point)
{
	t_point	pt;

	pt.min = plane_point.x - floorf(plane_point.x);
	pt.max = plane_point.z - floorf(plane_point.z);
	return (pt);
}

t_color	planar_pattern_at(const t_vec3 plane_point)
{
	t_checkers	checkers;
	t_point		pt;

	checkers = plane_uv_checkers();
	pt = planar_map(plane_point);
	return (uv_pattern_at(checkers, pt));
}
