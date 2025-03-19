/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:28:56 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/19 12:28:59 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"
#include "../../include/parse.h"

int	set_color(char *line, int start, t_vec3 *color)
{
	start = set_vec(line, start, color);
	if (!start)
		return (0);
	*color = vec3_normalize(*color);
	return (start);
}

int	set_vec(char *line, int start, t_vec3 *vec)
{
	int		i;
	char	*num;
	int		error;
	float	arr[3];
	int		comma;

	i = 0;
	while (i < 3)
	{
		if (i < 2)
			comma = line - ft_strchr(&line[start], ',');
		else
			comma = line - ft_strchr(&line[start], ' ');
		num = ft_substr(line, start, comma);
		if (!num)
			return (0);
		arr[i] = ft_strtod_s(num, &error);
		free(num);
		if (error)
			return (ft_parseerror("invalid number", line));
		start = comma + 1;
		++i;
	}
	*vec = vec3_new(arr[0], arr[1], arr[2]);
	return (comma + 1);
}
