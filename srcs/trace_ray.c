/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:55:47 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/16 15:19:28 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color	trace_ray(t_scene *scene, t_ray ray, t_intersection *out)
{
	if (find_closest_intersection(scene, ray, out))
		return (color_clamp(shade(scene, ray, *out)));
	return (color_new(0, 0, 0));
}
