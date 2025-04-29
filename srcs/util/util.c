/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:55:31 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/28 12:30:46 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

// Helper function to clamp a float value between min and max
float	clamp(float value, float min, float max)
{
	return (fminf(fmaxf(value, min), max));
}

float	image_aspect_ratio(t_scene *scene)
{
	return ((float)scene->image_width / (float)scene->image_height);
}

bool	equal(float a, float b)
{
	return (fabs(a - b) < EPSILON);
}

// binary exponentiation
float	powi(float x, int n)
{
	float	result;

	result = 1;
	if (n < 0)
	{
		x = 1.0f / x;
		n = -n;
	}
	while (n > 0)
	{
		if (n & 1)
		{
			result *= x;
		}
		x *= x;
		n >>= 1;
	}
	return (result);
}
