/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:48:11 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/10/27 14:55:58 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"
#include "../../include/parse.h"

int	set_light(char *line, t_light *light)
{
	int		i;
	int		error;
	char	*number;

	error = 0;
	i = set_vec(line, 2, &(light->pos));
	if (!i)
		return (0);
	i = ft_skip_space(line, i);
	number = ft_substr(line, i, ft_float_len(&line[i]));
	if (!number)
		return (ft_rperror("malloc)"));
	light->brightness = ft_strtof(number, &error);
	free(number);
	if (!light->brightness && error)
		return (ft_parseerror("invalid number", line, i));
	i = ft_skip_space(line, i + ft_float_len(&line[i]));
	if (line[i] != '\n')
		i = set_color(line, i, &(light->color));
	if (i == 0)
		return (0);
	i = ft_skip_space(line, i);
	if ((line[i] != '\n') && line[i] != '\0' && line[i] != EOF)
		return (ft_parseerror("expected end of line here", line, i - 1));
	return (1);
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
	free(number);
	if (!ambient->brightness && error)
		return (ft_parseerror("invalid number", line, i));
	i = ft_skip_space(line, i + floatlen);
	i = set_color(line, i, &(ambient->color));
	if (i == 0)
		return (0);
	i = ft_skip_space(line, i - 1);
	if ((line[i] != '\n') && line[i] != '\0' && line[i] != EOF)
		return (ft_parseerror("expected end of line here", line, i));
	return (i != 0);
}

int	set_camera(char *line, t_camera *camera)
{
	int		i;
	int		endi;
	char	*endp;

	i = 2;
	i = set_vec(line, i, &(camera->pos));
	if (!i)
		return (0);
	i = ft_skip_space(line, i);
	i = set_vec(line, i, &(camera->dir));
	if (!i)
		return (0);
	i = ft_skip_space(line, i);
	camera->fov = ft_strtoimax(&line[i], &endp, 10);
	if (!(camera->fov) && (*endp != '\n' && *endp != '\0' && *endp != EOF))
		return (ft_parseerror("invalid number", line, i));
	camera->up = vec3_new(0, 1, 0);
	camera->right = vec3_cross(camera->dir, camera->up);
	if (i == 0)
		return (0);
	endi = ft_skip_space(endp, 0);
	if ((*endp + endi != '\n') && *endp + endi != '\0' && *endp + endi != EOF)
		return (ft_parseerror("expected end of line here", line, i + 2));
	return (1);
}
