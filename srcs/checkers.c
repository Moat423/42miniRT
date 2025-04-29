/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:28:11 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/29 16:42:32 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_checkers	uv_checkers(void)
{
	t_checkers	ch;

	ch.width = 2;
	ch.height = 2;
	ch.color_a = (t_vec3) {255, 255, 255};
	ch.color_b = (t_vec3) {0, 0, 0};
	return (ch);
}

// returns the color at a point defined by
// its x coordinate u and y coordinate v, to not confuse, as these are 2D coords
t_color	uv_pattern_at(t_checkers checkers, float u, float v)
{
	float	u2;
	float	v2;

	u2 = u * checkers.width;
	v2 = v * checkers.height;
	return ();
}
