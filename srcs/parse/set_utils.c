/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:28:56 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/05 11:44:33 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"
#include "../../include/parse.h"

int	ft_set_texture_flag(int *i, char *line)
{
	*i = ft_skip_space(line, *i);
	if (line[*i] == 'c' && line[*i + 1] == 'h')
	{
		(*i) += 3;
		return (CHECKERS);
	}
	if (line[*i] == 'b' && line[*i + 1] == 'p')
	{
		(*i += 3);
		return (BUMP);
	}
	else
		return (NONE);
}

// returns 0 on error or the start + the length of the float 
// (up till first invalid char)
// it sets num to the float string called number that it mallocs.
// if the malloc fails, it returns 0
// if 
int	ft_substrtof(float *num, int start, char *line)
{
	int		floatlen;
	int		error;
	char	*number;

	floatlen = ft_float_len(&line[start]);
	number = ft_substr(line, start, floatlen);
	if (!number)
	{
		perror("malloc)");
		return (0);
	}
	*(num) = ft_strtof(number, &error);
	free(number);
	if (!*(num) && error)
		return (ft_parseerror("invalid number", line, start));
	return (ft_skip_space(line, start + floatlen));
}

int	set_color(char *line, int start, t_vec3 *color)
{
	start = set_vec(line, start, color);
	if (!start)
		return (0);
	*color = vec3_to_color(*color);
	return (start);
}

static int	parse_vec_component(char *line, int start, float *res, int is_last)
{
	int		error;
	int		len;
	char	delim;
	char	*num;

	len = ft_float_len(&line[start]);
	delim = line[start + len];
	if ((is_last && delim != ' ' && delim != '\n') || 
		(!is_last && delim != ','))
		return (ft_parseerror("invalid separation in vector-numbers",
				line, start + len));
	num = ft_substr(line, start, len);
	if (!num)
		return (ft_rperror("malloc"));
	error = 0;
	*res = ft_strtof(num, &error);
	free(num);
	if (error)
		return (ft_parseerror("invalid separation in vector-numbers",
				line, start + len));
	return (start + len + 1);
}

int	set_vec(char *line, int start, t_vec3 *vec)
{
	float	arr[3];
	int		i;

	i = -1;
	while (++i < 3)
	{
		start = parse_vec_component(line, start, &(arr[i]), i == 2);
		if (!start)
			return (0);
	}
	*vec = vec3_new(arr[0], arr[1], arr[2]);
	return (start);
}
