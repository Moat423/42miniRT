/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_png_texture.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:06:48 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/19 19:23:38 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parse.h"

int	set_png_texture(char *obj_line, mlx_texture_t **texture)
{
	char	*filename;
	int		position;

	position = 0;
	while (ft_isspace(obj_line[position]))
		++position;
	filename = ft_subword(obj_line);
	if (!filename)
		return (0);
	*texture = (mlx_load_png(filename));
	if (!*texture)
		return (0);
	while (obj_line[position] && (!ft_isspace(obj_line[position])
			|| obj_line[position] == '\n'))
		++position;
	return (position);
}
