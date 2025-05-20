/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_at.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:31:35 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/20 19:29:05 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"
#include <stdint.h>

t_color	uv_texture_at(mlx_texture_t *texture, t_point point)
{
	uint8_t			*pixel;
	unsigned int	x;
	unsigned int	y;

	point.u = point.u - floor(point.u);
	point.v = point.v - floor(point.v);
	x = (point.v) * (texture->height - 1);
	y = (1.0 - point.u) * (texture->width - 1);
	if (x < 0)
		x = 0;
	else if (x >= texture->height)
		x = x % (texture->width - 1);
	if (y < 0)
		y = 0;
	else if (y >= texture->width)
		y = y % (texture->height - 1);
	pixel = &texture->pixels[(int)(x * texture->width + y)
		* texture->bytes_per_pixel];
	return (color_from_rgb(pixel[0], pixel[1], pixel[2]));
}

t_color	sphere_texture_at(const t_vec3 sphere_point, const float radius,
							mlx_texture_t *texture)
{
	t_point		pt;

	pt = spherical_map(sphere_point, radius);
	return (uv_texture_at(texture, pt));
}
