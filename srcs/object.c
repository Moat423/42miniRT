/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:51:50 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/26 13:06:09 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color	object_color(t_object object)
{
	if (object.type == SPHERE)
		return (object.sphere->color);
	if (object.type == PLANE)
		return (object.plane->color);
	if (object.type == CYLINDER)
		return (object.cylinder->color);
	return (color_new(0, 0, 0));
}
