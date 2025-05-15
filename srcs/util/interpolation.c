/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpolation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:08:22 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/15 15:08:54 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT_types.h"

// bilinear interpolation function
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
