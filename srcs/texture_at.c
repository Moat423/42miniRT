/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_at.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:31:35 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/19 19:47:45 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"
#include <stdint.h>

t_color	uv_texture_at(mlx_texture_t *texture, t_point point)
{
	uint8_t	*pixel;

	pixel = &texture->pixels[(int)(point.u * texture->width + point.v)
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
