/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interval.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:17:32 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/02 11:26:58 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

t_interval	interval_new(float min, float max)
{
	t_interval	interval;

	interval.min = min;
	interval.max = max;
	return (interval);
}

bool	interval_contains(t_interval interval, float x)
{
	return (interval.min <= x && x <= interval.max);
}

bool	interval_surrounds(t_interval interval, float x)
{
	return (interval.min < x && x < interval.max);
}
