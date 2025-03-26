/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:48:11 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/26 12:48:13 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"
#include "../../include/parse.h"

int	set_light(char *line, t_light *light)
{
	int		i;
	int		error;
	char	*number;
	int		floatlen;

	i = 2;
	error = 0;
	i = set_vec(line, i, &(light->pos));
	if (!i)
		return (0);
	i = ft_skip_space(line, i);
	floatlen = ft_float_len(&line[i]);
	number = ft_substr(line, i, floatlen);
	if (!number)
		return (ft_rperror("malloc)"));
	light->brightness = ft_strtof(number, &error);
	if (!light->brightness && error)
		return (ft_parseerror("invalid number", line));
	i = ft_skip_space(line, i + floatlen);
	if (line[i] != '\n')
		i = set_color(line, i, &(light->color));
	return (i != 0);
}

int	set_ambient(char *line, t_ambient *ambient)
{
	int		i;
	int		floatlen;
	int		error;
	char	*number;

	i = 2;
	error = 0;
	floatlen = ft_float_len(&line[i]);
	number = ft_substr(line, i, floatlen);
	if (!number)
		return (ft_rperror("malloc)"));
	ambient->brightness = ft_strtof(number, &error);
	if (!ambient->brightness && error)
		return (ft_parseerror("invalid number", line));
	i = ft_skip_space(line, i + floatlen);
	i = set_color(line, i, &(ambient->color));
	return (i != 0);
}

int	set_camera(char *line, t_camera *camera)
{
	int	i;
	int	error;

	i = 2;
	error = 0;
	i = set_vec(line, i, &(camera->pos));
	if (!i)
		return (0);
	i = ft_skip_space(line, i);
	i = set_vec(line, i, &(camera->dir));
	if (!i)
		return (0);
	i = ft_skip_space(line, i);
	camera->fov = ft_strtoimax(&line[i], NULL, 10);
	if (!(camera->fov) && error)
		return (ft_parseerror("invalid number", line));
	camera->up = vec3_new(0, 1, 0);
	camera->right = vec3_cross(camera->dir, camera->up);
	return (1);
}
