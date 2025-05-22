/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:16:18 by kwurster          #+#    #+#             */
/*   Updated: 2025/05/22 12:31:09 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color	trace_ray(t_scene *scene, t_ray ray, t_intersection *out)
{
	if (find_closest_intersection(scene, ray, out))
		return (color_clamp(shade(scene, ray, *out)));
	return (color_new(0, 0, 0));
}

void	measure_render_time(t_minirt *minirt, double render_start)
{
	static bool	executed_once = false;

	minirt->render_time = mlx_get_time() - render_start;
	printf("Rendered frame in %f seconds\n", minirt->render_time);
	if (executed_once)
		return ;
	minirt->deferred_render = minirt->render_time > LONG_RENDER_TIME;
	executed_once = true;
	printf("Deferred render is active due to long render time.\n");
}
