/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:47:40 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/28 11:41:32 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT_types.h"

int	input_check(int argc)
{
	if (argc == 2)
		return (1);
	ft_fprintf(2, "usage: ./miniRT scene/scene-description.rt\n");
	ft_fprintf(2, 
		"scene-description.rt must be a valid file inside the 'scene' folder");
	return (0);
}

int	ft_parseerror(char *error, char *line, int position)
{
	int	i;

	if (!line)
	{
		ft_fprintf(2, "ERROR\n%s\n", error);
		return (0);
	}
	ft_fprintf(2, "ERROR\n%s\n   line:\n%s", error, line);
	i = 0;
	while (i != position)
	{
		ft_fprintf(2, "-");
		i++;
	}
	ft_fprintf(2, "^\n");
	return (0);
}

int	check_count(t_scene *scene, int camera, int ambient)
{
	if (camera != 1)
		ft_fprintf (2, "Need one single camera (C), have %d\n", camera);
	else if (ambient != 1)
		ft_fprintf (2, "Need one single ambient light (A), have %d\n", ambient);
	else if (scene->light_count == 0)
		ft_fprintf (2, "Need at least one single light (L), have 0\n");
	else if (scene->light_count > MAX_LIGHTS)
	{
		ft_fprintf(2, "Have too many lights (L)\n");
		ft_fprintf(2, "current maximum light count: %d \n", MAX_LIGHTS);
	}
	else
		return (1);
	scene->light_count = 0;
	return (0);
}
