/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_checkers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:29:46 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/16 18:51:23 by moat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_checkers	default_uv_checkers(void)
{
	t_checkers	ch;

	ch.width = 16;
	ch.height = 8;
	ch.color_a = (t_vec3){1, 1, 1};
	ch.color_b = (t_vec3){0, 0, 0};
	return (ch);
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
	pt.u = 1 - (raw_u + 0.5);
	pt.v = 1 - phi / M_PI;
	return (pt);
}

t_color	sphere_pattern_at(const t_vec3 sphere_point, const float radius, t_checkers *checker_pattern)
{
	t_checkers	checkers;
	t_point		pt;

	checkers = *checker_pattern;
	pt = spherical_map(sphere_point, radius);
	return (uv_pattern_at(checkers, pt));
}
