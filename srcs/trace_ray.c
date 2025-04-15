/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:55:47 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/26 13:11:07 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color	trace_ray(t_scene *scene, t_ray ray)
{
	t_intersection	intersection;

	(void)intersection;
	(void)scene;
	(void)ray;
	if (find_closest_intersection(scene, ray, &intersection))
		return (color_clamp(shade(scene, ray, intersection)));
		//return (calculate_lighting(intersection, scene));
	//else
	return (color_new(0, 0, 0));
}
