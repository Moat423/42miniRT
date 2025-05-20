/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_at.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:31:35 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/20 19:00:49 by lmeubrin         ###   ########.fr       */
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
	y = (1.0 - point.u) * (texture->width - 1);
	x = (point.v) * (texture->height - 1);
    x = (x < 0) ? 0 : ((x >= texture->height) ? texture->width - 1 : x);
    y = (y < 0) ? 0 : ((y >= texture->width) ? texture->height - 1 : y);
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
